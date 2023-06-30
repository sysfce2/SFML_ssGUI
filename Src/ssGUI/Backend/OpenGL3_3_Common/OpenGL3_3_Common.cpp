#include "ssGUI/Backend/OpenGL3_3_Common/OpenGL3_3_Common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/HelperClasses/ImageUtil.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include "ssGUI/DataClasses/DrawingEntity.hpp"
#include <memory>

#define GL_CHECK_ERROR(x)\
{\
    GLenum err;\
    x;\
    if((err = glGetError()) != GL_NO_ERROR)\
    {\
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed: "<<err);\
        ssLOG_EXIT_PROGRAM(1);\
    }\
}

GLuint PRIMITIVE_RESTART = 543210; // magic value


namespace ssGUI
{

namespace Backend
{
    const std::string OpenGL3_3_Common::VertShader = 
    R"(
        #version 330 core
        
        layout (location = 0) in vec2 vertPos;
        layout (location = 1) in vec4 vertColor;
        layout (location = 2) in vec3 vertTexCoord;
        layout (location = 3) in vec2 vertTexTopLeft;
        layout (location = 4) in vec2 vertTexBotRight;

        out vec4 fragColor;
        out vec3 fragTexCoord;
        out vec2 fragTexTopLeft;
        out vec2 fragTexBotRight;

        uniform mat4 projMatrix;

        void main()
        {
            gl_Position = vec4(vertPos, 0.0, 1.0);
            //gl_Position = projMatrix * vec4(vertPos, 0.0, 1.0);
            
            fragColor = vertColor;
            fragTexCoord = vertTexCoord;
            fragTexTopLeft = vertTexTopLeft;
            fragTexBotRight = vertTexBotRight;
        }
    )";
    
    const std::string OpenGL3_3_Common::FragShader = 
    R"(
        #version 330 core
        
        #extension GL_ARB_texture_query_lod : enable

        out vec4 outColor;
        
        in vec4 fragColor;
        in vec3 fragTexCoord;
        in vec2 fragTexTopLeft;
        in vec2 fragTexBotRight;

        uniform sampler2DArray ourTexture;

        //From: https://stackoverflow.com/questions/24388346/how-to-access-automatic-mipmap-level-in-glsl-fragment-shader-texture
        // Does not take into account GL_TEXTURE_MIN_LOD/GL_TEXTURE_MAX_LOD/GL_TEXTURE_LOD_BIAS,
        // nor implementation-specific flexibility allowed by OpenGL spec
        float mip_map_level(in vec2 texture_coordinate) // in texel units
        {
            vec2  dx_vtc        = dFdx(texture_coordinate);
            vec2  dy_vtc        = dFdy(texture_coordinate);
            float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
            float mml = 0.5 * log2(delta_max_sqr);
            return max( 0, mml ); // Thanks @Nims
        }

        void main()
        {
            //if(fragTexTopLeft.x < 0)
            {
                //outColor = fragColor / 255.f;
                outColor = vec4(1, 0, 0, 1);
                return;
            }
            
            //Normalizing texture coordinates
            vec3 fragTexCoordNormalized = fragTexCoord;
            vec2 fragTexTopLeftNormalized = fragTexTopLeft;
            vec2 fragTexBotRightNormalized = fragTexBotRight;
            
            fragTexCoordNormalized.xy /= textureSize(ourTexture, 0).xy;
            fragTexTopLeftNormalized.xy /= textureSize(ourTexture, 0).xy;
            fragTexBotRightNormalized.xy /= textureSize(ourTexture, 0).xy;
            
            float mipmapLevel = 0;
        
            #ifdef GL_ARB_texture_query_lod
                mipmapLevel = textureQueryLOD(ourTexture, vec2(fragTexCoordNormalized.x, fragTexCoordNormalized.y)).x;
            #else
                mipmapLevel = mip_map_level(fragTexCoordNormalized.xy * textureSize(ourTexture, 0).xy);
            #endif
            
            int roundedMipmapLevel = int(mipmapLevel + 0.5f);
            
            //If mipmap level is 0, don't need to do anything
            if(roundedMipmapLevel == 0)
            {
                outColor = texture(ourTexture, fragTexCoordNormalized) * fragColor;
                return;
            }
            
            //Otherwise get the localized UV
            vec2 localTexCoord = (fragTexCoordNormalized.xy - fragTexTopLeftNormalized) / (fragTexBotRightNormalized - fragTexTopLeftNormalized);
            
            //And multiply by 0.5 ^ mipmap level
            float mipmapMultiplier = pow(0.5f, float(roundedMipmapLevel));
            
            //The coordinate of the mipmap in y can be found by using the nth partial sum of a geometric sequence
            //  with this (modified) formula: (1 - 0.5 ^ mipmapLevel) / (1 - 0.5) - 1
            float mipmapYOffsetMultiplier = (1 - mipmapMultiplier) / 0.5f - 1;
            
            vec3 mipmapTexCoord = vec3( fragTexBotRightNormalized.x + localTexCoord.x * mipmapMultiplier, 
                                        fragTexTopLeftNormalized.y + mipmapYOffsetMultiplier + localTexCoord.y * mipmapMultiplier,
                                        fragTexCoordNormalized.z);
        
            outColor = texture(ourTexture, mipmapTexCoord) * fragColor;
        }  
    )";


    bool OpenGL3_3_Common::DrawShape(   const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const uint32_t character,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int characterSize)
    {
        if(!font.IsValid())
            return false;
    
        auto& rawFont = const_cast<ssGUI::Backend::BackendFontInterface&>(font);

        CharTextureIdentifier curIdentifier = CharTextureIdentifier(&rawFont, characterSize, character);
        
        if(MappedFontIds.find(curIdentifier) == MappedFontIds.end())
        {
            if(!AddFontCache(curIdentifier))
                return false;
        }
        
        int atlasId = MappedFontIds.at(curIdentifier);
        
        DynamicImageAtlas::ImageAtlasImageInfo returnInfo;
        if(!CurrentImageAtlas->GetImageInfo(atlasId, returnInfo))
            return false;

        Vertices.insert(Vertices.end(), vertices.begin(), vertices.end());
        Colors.insert(Colors.end(), colors.begin(), colors.end());
        
        GLuint startIndex = TexCoords.size();
        TexCoords.insert(TexCoords.end(), vertices.size(), returnInfo.LocationInPixel);
        
        for(int i = startIndex; i < TexCoords.size(); i++)
            TexCoords.at(i) += glm::vec3(texCoords.at(i - startIndex), 0);
        
        TextureTopLeftCoords.insert(TextureTopLeftCoords.end(), vertices.size(), returnInfo.LocationInPixel);
        TextureBotRightCoords.insert(TextureBotRightCoords.end(), vertices.size(), returnInfo.LocationInPixel + glm::ivec3(returnInfo.ImageSizeInPixel, 0));
        
        startIndex = Vertices.size() - vertices.size();
        for(int i = 0; i < vertices.size(); i++)
            Idx.push_back(startIndex + i);
        
        Idx.push_back(PRIMITIVE_RESTART);
        
        return true;        
    }

    bool OpenGL3_3_Common::DrawShape(   const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const ssGUI::Backend::BackendImageInterface& image)
    {
        if(!image.IsValid())
            return false;
        
        if(MappedImgIds.find((ssGUI::Backend::BackendImageInterface*)&image) == MappedImgIds.end())
        {
            if(!AddImageCache((ssGUI::Backend::BackendImageInterface*)&image))
                return false;
        }
        
        int atlasId = MappedImgIds.at((ssGUI::Backend::BackendImageInterface*)&image);
        
        DynamicImageAtlas::ImageAtlasImageInfo returnInfo;
        if(!CurrentImageAtlas->GetImageInfo(atlasId, returnInfo))
            return false;
        
        //Vertices.insert(Vertices.end(), vertices.begin(), vertices.end());
        
        for(int i = 0; i < vertices.size(); i++)
        {
            Vertices.push_back(vertices.at(i) / glm::vec2(CurrentMainWindow->GetWindowSize()));
        }
        
        Colors.insert(Colors.end(), colors.begin(), colors.end());
        
        GLuint startIndex = TexCoords.size();
        TexCoords.insert(TexCoords.end(), vertices.size(), returnInfo.LocationInPixel);
        
        for(int i = startIndex; i < TexCoords.size(); i++)
            TexCoords.at(i) += glm::vec3(texCoords.at(i - startIndex), 0);

        TextureTopLeftCoords.insert(TextureTopLeftCoords.end(), vertices.size(), returnInfo.LocationInPixel);
        TextureBotRightCoords.insert(TextureBotRightCoords.end(), vertices.size(), returnInfo.LocationInPixel + glm::ivec3(returnInfo.ImageSizeInPixel, 0));
        
        startIndex = Vertices.size() - vertices.size();
        for(int i = 0; i < vertices.size(); i++)
            Idx.push_back(startIndex + i);
        
        Idx.push_back(PRIMITIVE_RESTART);


        #if 0
            CurrentMainWindow->SetGLContext();

            if(!image.IsValid())
                return false;

            //GL_CHECK_ERROR( glUseProgram(ProgramId); );
            
            AddImageCache(const_cast<ssGUI::Backend::BackendImageInterface*>(&image));
            
            GLuint debugFBO = 0;
            GL_CHECK_ERROR( glGenFramebuffers(1, &debugFBO) );
            GL_CHECK_ERROR( glBindFramebuffer(GL_READ_FRAMEBUFFER, debugFBO) );
            
            GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_READ_FRAMEBUFFER, 
                                                        GL_COLOR_ATTACHMENT0, 
                                                        CachedImages,
                                                        0,
                                                        0) );
                                                        
            GL_CHECK_ERROR( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0) );
            
            DynamicImageAtlas::ImageAtlasImageInfo imgInfo;

            if(!CurrentImageAtlas->GetImageInfo(MappedImgIds.at(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)), imgInfo))
                return false;
            
            ssLOG_LINE("imgInfo: "<<imgInfo);
            ssLOG_LINE("image.GetSize(): "<<image.GetSize());
            
            GL_CHECK_ERROR( glBlitFramebuffer(  imgInfo.LocationInPixel.x,
                                                imgInfo.LocationInPixel.y, 
                                                imgInfo.ImageSizeInPixel.x * 1.5f, 
                                                imgInfo.ImageSizeInPixel.y, 
                                                0, 
                                                CurrentMainWindow->GetWindowSize().y - 50, 
                                                imgInfo.ImageSizeInPixel.x * 1.5f, 
                                                CurrentMainWindow->GetWindowSize().y - imgInfo.ImageSizeInPixel.y - 50,
                                                GL_COLOR_BUFFER_BIT, 
                                                GL_NEAREST) );
            
            
            
            GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_READ_FRAMEBUFFER, 
                                                        GL_COLOR_ATTACHMENT0, 
                                                        0,
                                                        0,
                                                        0) );
            
            GL_CHECK_ERROR( glBindFramebuffer(GL_READ_FRAMEBUFFER, 0) );
            GL_CHECK_ERROR( glDeleteFramebuffers(1, &debugFBO) );

            return true;
            
            glm::ivec2 imgSize = image.GetSize();
            
            //If unable to add to cache, return
            //if(ImageTextures.find(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)) == ImageTextures.end())
            //    return false;
            //else
            //    GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, ImageTextures[const_cast<ssGUI::Backend::BackendImageInterface*>(&image)]); );

            if(MappedImgIds.find(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)) == MappedImgIds.end())
                return false;

            //DynamicImageAtlas::ImageAtlasImageInfo imgInfo;

            if(!CurrentImageAtlas->GetImageInfo(MappedImgIds.at(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)), imgInfo))
                return false;

            //if(CurrentImageAtlas->RequestImage(ImageAtlasImageInfo imgInfo, int &returnId).find(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)) == ImageTextures.end())
            //    return false;

            GL_CHECK_ERROR( glEnable(GL_TEXTURE_3D); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, CachedImages) );

            //render it
            GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D); );
            GL_CHECK_ERROR( glEnable(GL_BLEND); );
            GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

            glBegin(GL_TRIANGLE_FAN);

            for(int i = 0; i < vertices.size(); i++)
            {
                glm::vec2 texCoord = texCoords[i];
                texCoord.x /= imgSize.x;
                texCoord.y /= imgSize.y;
                texCoord *= glm::vec2(imgInfo.ImageSizeInPixel) / glm::vec2(CurrentImageAtlas->GetAtlasSize());
                texCoord += glm::vec2(imgInfo.LocationInPixel.x, imgInfo.LocationInPixel.y) / glm::vec2(CurrentImageAtlas->GetAtlasSize());

                ssLOG_LINE("pre texCoords[" << i << "]: "<<texCoords[i]);
                ssLOG_LINE("post texCoord[" << i << "]: "<<texCoord);

                //glTexCoord2f(texCoord.x, texCoord.y);
                glTexCoord3f(texCoord.x, texCoord.y, imgInfo.LocationInPixel.z);

                glColor4ub(127, colors[i].g, colors[i].b, colors[i].a);

                //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
                glVertex3f(round(vertices[i].x), round(vertices[i].y), 0);
            }

            GL_CHECK_ERROR( glEnd(); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, 0); );
            GL_CHECK_ERROR( glFlush(); );
        #endif

        return true;    
    }


    bool OpenGL3_3_Common::DrawShape(   const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colors)
    {
        Vertices.insert(Vertices.end(), vertices.begin(), vertices.end());
        //for(int i = 0; i < vertices.size(); i++)
        //{
        //    Vertices.push_back(vertices.at(i) / glm::vec2(CurrentMainWindow->GetWindowSize()));
        //}
        
        Colors.insert(Colors.end(), colors.begin(), colors.end());
        TexCoords.insert(TexCoords.end(), vertices.size(), glm::vec3(-1, -1, -1));
        TextureTopLeftCoords.insert(TextureTopLeftCoords.end(), vertices.size(), glm::vec3(-1, -1, -1));
        TextureBotRightCoords.insert(TextureBotRightCoords.end(), vertices.size(), glm::vec3(-1, -1, -1));
        
        GLuint startIndex = Vertices.size() - vertices.size();
        for(int i = 0; i < vertices.size(); i++)
            Idx.push_back(startIndex + i);
        
        Idx.push_back(PRIMITIVE_RESTART);


        return true;
    }
    
    bool OpenGL3_3_Common::OnNewAtlasRequest()
    {
        //TODO
        return false;
    }
    
    void OpenGL3_3_Common::SaveLastViewport()
    {
        GL_CHECK_ERROR( glGetIntegerv(GL_VIEWPORT, LastViewportProperties) );
    }
    
    void OpenGL3_3_Common::LoadLastViewport()
    {
        GL_CHECK_ERROR( glViewport( LastViewportProperties[0], 
                                    LastViewportProperties[1], 
                                    (GLsizei)LastViewportProperties[2], 
                                    (GLsizei)LastViewportProperties[3]) );
    }
    
    template<typename T>
    bool OpenGL3_3_Common::AddDrawingCache(std::unordered_map<T, int>& cachedIds, T key, glm::ivec2 textureSize, const void* rgba32Pixels)
    {
        if(cachedIds.find(key) != cachedIds.end())
            return false;

        CurrentMainWindow->SetGLContext();

        {
            //Allocate space on atlas
            DynamicImageAtlas::ImageAtlasImageInfo imgInfo;
            imgInfo.ImageSizeInPixel = textureSize;
            imgInfo.HasMipmap = true;
            int imgId;
            
            if(!CurrentImageAtlas->RequestImage(imgInfo, imgId))
            {
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to request atlas image");
                return false;
            }
            
            if(!CurrentImageAtlas->GetImageInfo(imgId, imgInfo))
            {
                ssGUI_ERROR("Internal error, ImageAtlas failed internally");
                return false;
            }
            
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, CachedImages) );
            
            //Write the image data to the texture
            GL_CHECK_ERROR( glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                                            0, 
                                            imgInfo.LocationInPixel.x, 
                                            imgInfo.LocationInPixel.y, 
                                            imgInfo.LocationInPixel.z,
                                            imgInfo.ImageSizeInPixel.x,
                                            imgInfo.ImageSizeInPixel.y,
                                            1,
                                            GL_RGBA,
                                            GL_UNSIGNED_BYTE,
                                            rgba32Pixels) );
                                            
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, 0) );
            
            #if 0
            //================================================================================
            //DEBUG
            //================================================================================
            GLuint debugFBO = 0;
            GL_CHECK_ERROR( glGenFramebuffers(1, &debugFBO) );
            GL_CHECK_ERROR( glBindFramebuffer(GL_FRAMEBUFFER, debugFBO) );
            
            
            GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_FRAMEBUFFER, 
                                                        GL_COLOR_ATTACHMENT0, 
                                                        CachedImages,
                                                        0,
                                                        imgInfo.LocationInPixel.z) );
                                                        
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to attach to framebuffer");
            
            uint8_t fboPixels[5 * 4];
            GL_CHECK_ERROR( glReadPixels(0, 0, 5, 1, GL_RGBA,  GL_UNSIGNED_BYTE, fboPixels) );
            
            
            
            GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_FRAMEBUFFER, 
                                                        GL_COLOR_ATTACHMENT0, 
                                                        0,
                                                        0,
                                                        0) );
            
            for(int i = 0; i < 5; i++)
            {
                ssLOG_LINE("fboPixels[" << i << "]: "<<(int)fboPixels[i * 4 + 0] <<", "<<(int)fboPixels[i * 4 + 1] <<", "<<(int)fboPixels[i * 4 + 2]<<", "<<(int)fboPixels[i * 4 + 3]);
            }
            
            GL_CHECK_ERROR( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
            
            GL_CHECK_ERROR( glDeleteFramebuffers(1, &debugFBO) );
            
            //================================================================================
            //DEBUG ENDS
            //================================================================================
            #endif
            
            cachedIds[key] = imgId;
            
            if(textureSize.x == 1 || textureSize.y == 1)
                return false;

            glm::ivec2 lastMipmapSize = textureSize;
            glm::ivec2 lastMipmampPixelPosOffset = glm::ivec2(0, 0);
            glm::ivec2 currentMipmampPixelPosOffset = glm::ivec2(lastMipmapSize.x, 0);
            
            const int MAX_LOOP = 100;
            const int READ_FB = 0;
            const int DRAW_FB = 1;
            
            GLuint readDrawFramebuffers[2] = {0, 0};
            
            GL_CHECK_ERROR( glGenFramebuffers(2, readDrawFramebuffers) );
            GL_CHECK_ERROR( glBindFramebuffer(GL_READ_FRAMEBUFFER, readDrawFramebuffers[READ_FB]) );
            GL_CHECK_ERROR( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, readDrawFramebuffers[DRAW_FB]) );

            GLuint mipmapRenderbuffer = 0;
            GL_CHECK_ERROR( glGenRenderbuffers(1, &mipmapRenderbuffer) );
            GL_CHECK_ERROR( glBindRenderbuffer(GL_RENDERBUFFER, mipmapRenderbuffer) );

            //Finally, generates mipmap for it
            for(int i = 0; i < MAX_LOOP; i++)
            {
                //Last mipmap, no need to continue
                if(lastMipmapSize.x / 2 <= 0 || lastMipmapSize.y / 2 <= 0)
                    break;
            
                //Generate mipmap to a render buffer    
                GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_READ_FRAMEBUFFER, 
                                                            GL_COLOR_ATTACHMENT0, 
                                                            CachedImages,
                                                            0,
                                                            imgInfo.LocationInPixel.z) );

                if(glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to attach to framebuffer");

                GL_CHECK_ERROR( glBindRenderbuffer(GL_RENDERBUFFER, mipmapRenderbuffer) );
                GL_CHECK_ERROR( glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, lastMipmapSize.x / 2, lastMipmapSize.y / 2) );
                GL_CHECK_ERROR( glBindRenderbuffer(GL_RENDERBUFFER, 0) );

                GL_CHECK_ERROR( glFramebufferRenderbuffer(  GL_DRAW_FRAMEBUFFER, 
                                                            GL_COLOR_ATTACHMENT0, 
                                                            GL_RENDERBUFFER, 
                                                            mipmapRenderbuffer) );
                
                if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to attach to framebuffer");

                GL_CHECK_ERROR( glBlitFramebuffer(  lastMipmampPixelPosOffset.x, 
                                                    lastMipmampPixelPosOffset.y, 
                                                    lastMipmampPixelPosOffset.x + lastMipmapSize.x, 
                                                    lastMipmampPixelPosOffset.y + lastMipmapSize.y, 
                                                    0, 
                                                    0, 
                                                    lastMipmapSize.x / 2, 
                                                    lastMipmapSize.y / 2,
                                                    GL_COLOR_BUFFER_BIT, 
                                                    GL_LINEAR) );

                //Then copy the mipmap render buffer to the atlas
                GL_CHECK_ERROR( glFramebufferRenderbuffer(  GL_READ_FRAMEBUFFER, 
                                                            GL_COLOR_ATTACHMENT0, 
                                                            GL_RENDERBUFFER, 
                                                            mipmapRenderbuffer) );

                if(glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to attach to framebuffer");

                GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_DRAW_FRAMEBUFFER, 
                                                            GL_COLOR_ATTACHMENT0, 
                                                            CachedImages,
                                                            0,
                                                            imgInfo.LocationInPixel.z) );

                if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to attach to framebuffer");

                GL_CHECK_ERROR( glBlitFramebuffer(  0, 
                                                    0, 
                                                    lastMipmapSize.x / 2, 
                                                    lastMipmapSize.y / 2,
                                                    currentMipmampPixelPosOffset.x,
                                                    currentMipmampPixelPosOffset.y, 
                                                    currentMipmampPixelPosOffset.x + lastMipmapSize.x / 2, 
                                                    currentMipmampPixelPosOffset.y + lastMipmapSize.y / 2, 
                                                    GL_COLOR_BUFFER_BIT, 
                                                    GL_NEAREST) );
                
                lastMipmapSize /= 2;
                lastMipmampPixelPosOffset = currentMipmampPixelPosOffset;
                currentMipmampPixelPosOffset += glm::ivec2(0, lastMipmapSize.y);
                
                if(i == MAX_LOOP - 1)
                {
                    ssGUI_ERROR(ssGUI_BACKEND_TAG, "MAX_LOOP reached, internal error!!");
                    GL_CHECK_ERROR( glDeleteFramebuffers(2, readDrawFramebuffers) );
                    return false;
                }
            }
            
            //Cleaning up framebuffers and render buffer
            GL_CHECK_ERROR( glFramebufferRenderbuffer(  GL_READ_FRAMEBUFFER, 
                                                        GL_COLOR_ATTACHMENT0, 
                                                        GL_RENDERBUFFER, 
                                                        0) );

            GL_CHECK_ERROR( glFramebufferTextureLayer(  GL_DRAW_FRAMEBUFFER, 
                                                        GL_COLOR_ATTACHMENT0, 
                                                        0,
                                                        0,
                                                        0) );
            
            GL_CHECK_ERROR( glBindFramebuffer(GL_READ_FRAMEBUFFER, 0) );
            GL_CHECK_ERROR( glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0) );

            GL_CHECK_ERROR( glDeleteRenderbuffers(1, &mipmapRenderbuffer) );
            GL_CHECK_ERROR( glDeleteFramebuffers(2, readDrawFramebuffers) );
        }
    
        return true;
    }
    
    template<typename T>
    bool OpenGL3_3_Common::RemoveDrawingCache(std::unordered_map<T, int>& cachedIds, T key)
    {
        auto it = cachedIds.find(key); 
        
        if(it == cachedIds.end())
            return false;
            
        CurrentImageAtlas->RemoveImage(it->second);
    
        cachedIds.erase(it);
    
        return true;
    }
    
    //Explicit template instantiations
    template bool OpenGL3_3_Common::AddDrawingCache(std::unordered_map<ssGUI::Backend::BackendImageInterface*, int>& cachedIds, 
                                                    ssGUI::Backend::BackendImageInterface* key,
                                                    glm::ivec2 textureSize, 
                                                    const void* rgba32Pixels);
    
    template bool OpenGL3_3_Common::AddDrawingCache(std::unordered_map<OpenGL3_3_Common::CharTextureIdentifier, int>& cachedIds, 
                                                    OpenGL3_3_Common::CharTextureIdentifier key,
                                                    glm::ivec2 textureSize, 
                                                    const void* rgba32Pixels);

    template bool OpenGL3_3_Common::RemoveDrawingCache( std::unordered_map<ssGUI::Backend::BackendImageInterface*, int>& cachedIds, 
                                                        ssGUI::Backend::BackendImageInterface* key);
    
    
    template bool OpenGL3_3_Common::RemoveDrawingCache( std::unordered_map<OpenGL3_3_Common::CharTextureIdentifier*, int>& cachedIds, 
                                                        OpenGL3_3_Common::CharTextureIdentifier* key);

    OpenGL3_3_Common::OpenGL3_3_Common( BackendMainWindowInterface* mainWindow,
                                        BackendDrawingInterface* drawingBackend) :  ProgramId(0),
                                                                                    CachedImages(0),
                                                                                    VAO(0),
                                                                                    EBO(0),
                                                                                    VertsVBO(0),
                                                                                    ColorsVBO(0),
                                                                                    TexCoordsVBO(0),
                                                                                    TopLeftTexCoordsVBO(0),
                                                                                    BotRightTexCoordsVBO(0),
                                                                                    LastViewportProperties(),
                                                                                    Vertices(),
                                                                                    Colors(),
                                                                                    TexCoords(),
                                                                                    TextureTopLeftCoords(),
                                                                                    TextureBotRightCoords(),
                                                                                    Idx(),
                                                                                    CurrentMainWindow(nullptr),
                                                                                    CurrentDrawingBackend(nullptr),
                                                                                    LastMainWindowSize(),
                                                                                    CurrentImageAtlas(nullptr),
                                                                                    MappedImgIds(),
                                                                                    MappedFontIds(),
                                                                                    VERT_POS_INDEX(0),
                                                                                    VERT_COLOR_INDEX(1),
                                                                                    VERT_TEX_COORD_INDEX(2),
                                                                                    VERT_TEX_TOP_LEFT_INDEX(3),
                                                                                    VERT_TEX_BOT_RIGHT_INDEX(4)
    {
        CurrentMainWindow = mainWindow;
        CurrentDrawingBackend = drawingBackend;

        mainWindow->SetGLContext();
    
        GLint maxTextureSize = 0;
        GLint maxLayerSize = 0;
        
        GL_CHECK_ERROR( glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize) );
        GL_CHECK_ERROR( glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayerSize) );
        
        ssLOG_LINE("maxTextureSize: "<<maxTextureSize);
        ssLOG_LINE("maxLayerSize: "<<maxLayerSize);
        
        GLint success = GL_FALSE;
        char infoLog[512] { 0 };
        
        // vertex Shader
        GLuint vertexShaderId = 0;
        GL_CHECK_ERROR( vertexShaderId = glCreateShader(GL_VERTEX_SHADER) );
        const char* vertShaderP = VertShader.c_str();
        GL_CHECK_ERROR( glShaderSource(vertexShaderId, 1, &vertShaderP, NULL) );
        GL_CHECK_ERROR( glCompileShader(vertexShaderId) );
        
        {
            GL_CHECK_ERROR( glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success) );
            if(success != GL_TRUE)
            {
                GL_CHECK_ERROR( glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog) );
                ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to compile shaders:");
                ssGUI_ERROR(ssGUI_BACKEND_TAG, infoLog);
                ssLOG_EXIT_PROGRAM(1);
            };
        }
        
        // fragment Shader
        GLuint fragmentShaderId = 0;
        GL_CHECK_ERROR( fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER) );
        const char* fargShaderP = FragShader.c_str();
        GL_CHECK_ERROR( glShaderSource(fragmentShaderId, 1, &fargShaderP, NULL) );
        GL_CHECK_ERROR( glCompileShader(fragmentShaderId) );

        {
            GL_CHECK_ERROR( glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success) );
            if(success != GL_TRUE)
            {
                GL_CHECK_ERROR( glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog) );
                ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to compile shaders:");
                ssGUI_ERROR(ssGUI_BACKEND_TAG, infoLog);
                ssLOG_EXIT_PROGRAM(1);
            };
        }

        // shader Program
        GL_CHECK_ERROR( ProgramId = glCreateProgram() );
        
        GL_CHECK_ERROR( glAttachShader(ProgramId, vertexShaderId) );
        GL_CHECK_ERROR( glAttachShader(ProgramId, fragmentShaderId) );
        //GL_CHECK_ERROR( glLinkProgram(ProgramId) );
        glLinkProgram(ProgramId);
        
        // print linking errors if any
        glGetProgramiv(ProgramId, GL_LINK_STATUS, &success);
        {
            GL_CHECK_ERROR( glGetProgramiv(ProgramId, GL_LINK_STATUS, &success) );
            if(success != GL_TRUE)
            {
                GL_CHECK_ERROR( glGetProgramInfoLog(ProgramId, 512, NULL, infoLog) );
                ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to compile shaders:");
                ssGUI_ERROR(ssGUI_BACKEND_TAG, infoLog);
                ssLOG_EXIT_PROGRAM(1);
            };
        }
        
        GL_CHECK_ERROR( glDetachShader(ProgramId, vertexShaderId) );
        GL_CHECK_ERROR( glDetachShader(ProgramId, fragmentShaderId) );
        
        // delete the shaders as they're linked into our program now and no longer necessary
        GL_CHECK_ERROR( glDeleteShader(vertexShaderId) );
        GL_CHECK_ERROR( glDeleteShader(fragmentShaderId) );
        
        GL_CHECK_ERROR( glEnable(GL_TEXTURE_3D) );
        
        GL_CHECK_ERROR( glGenTextures(1, &CachedImages) );
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, CachedImages) );
        
        GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
        GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
        
        GL_CHECK_ERROR( glTexImage3D(   GL_TEXTURE_2D_ARRAY, 
                                        0, 
                                        GL_RGBA8, 
                                        maxTextureSize, 
                                        maxTextureSize,
                                        1,
                                        0,
                                        GL_RGBA,
                                        GL_UNSIGNED_BYTE,
                                        NULL) );
        
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, 0) );
        
        //Generate ID for EBO that uses it
        //GL_CHECK_ERROR( glGenBuffers(1, &EBO) );
        
        //Generate ID for VBOs for vertex pos, colors, UVs and UseUVs flag
        GL_CHECK_ERROR( glGenBuffers(1, &VertsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &ColorsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &TexCoordsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &TopLeftTexCoordsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &BotRightTexCoordsVBO) );
        
        //Generate ID for VAO and bind VBOs and EBO created above to this VAO
        GL_CHECK_ERROR( glGenVertexArrays(1, &VAO) );
        
        //GLint location = 0;
        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_POS_INDEX, "vertPos") );
        //GL_CHECK_ERROR( location = glGetAttribLocation(ProgramId, "vertPos"))
        GL_CHECK_ERROR( glBindVertexArray(VAO) );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, VertsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_POS_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_POS_INDEX) );
        
        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_COLOR_INDEX, "vertColor") );
        //GL_CHECK_ERROR( location = glGetAttribLocation(ProgramId, "vertColor"))
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, ColorsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_COLOR_INDEX) );
        
        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_TEX_COORD_INDEX, "vertTexCoord") );
        //GL_CHECK_ERROR( location = glGetAttribLocation(ProgramId, "vertTexCoord"))
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, TexCoordsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_TEX_COORD_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_TEX_COORD_INDEX) );

        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_TEX_TOP_LEFT_INDEX, "vertTexTopLeft") );
        //GL_CHECK_ERROR( location = glGetAttribLocation(ProgramId, "vertTexTopLeft"))
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, TopLeftTexCoordsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_TEX_TOP_LEFT_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_TEX_TOP_LEFT_INDEX) );

        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_TEX_BOT_RIGHT_INDEX, "vertTexBotRight") );
        //GL_CHECK_ERROR( location = glGetAttribLocation(ProgramId, "vertTexBotRight"))
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, BotRightTexCoordsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_TEX_BOT_RIGHT_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_TEX_BOT_RIGHT_INDEX) );

        GL_CHECK_ERROR( glBindVertexArray(0) );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, 0) );

        CurrentImageAtlas = new ssGUI::Backend::DynamicImageAtlas(  glm::ivec2(maxTextureSize, maxTextureSize), 
                                                                    glm::ivec2(64, 64), 
                                                                    std::bind(&OpenGL3_3_Common::OnNewAtlasRequest, this));
        
        //// set the texture wrapping/filtering options (on the currently bound texture object)
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //// load and generate the texture
        //int width, height, nrChannels;
    }
    
    OpenGL3_3_Common::~OpenGL3_3_Common()
    {
        delete CurrentImageAtlas;
    }
    
    glm::mat4x4 OpenGL3_3_Common::UpdateViewPortAndModelView(glm::ivec2 widthHeight)
    {
        if(CurrentMainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return glm::mat4x4();
        }

        //if(CurrentMainWindow->GetRenderSize() == LastMainWindowSize)
        //    return glm::mat4x4;

        //LastMainWindowSize = CurrentMainWindow->GetRenderSize();
        
        CurrentMainWindow->SetGLContext();
        //glm::ivec2 renderSize = CurrentMainWindow->GetRenderSize();
        GL_CHECK_ERROR( glViewport(0, 0, widthHeight.x, widthHeight.y); );
        
        //These are for glVertex calls
        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW););
        GL_CHECK_ERROR( glLoadIdentity(); );
        glm::mat4x4 orthoMat = glm::ortho<float>(0.f, widthHeight.x, widthHeight.y, 0.f, 0, 10);
        GL_CHECK_ERROR( glMultMatrixf(glm::value_ptr(orthoMat)); );
        
        ssLOG_LINE("Width: "<<widthHeight.x);
        ssLOG_LINE("Height: "<<widthHeight.y);
        {
            ssLOG_LINE("orthoMat: "<<orthoMat[0][0]<<", "<<orthoMat[1][0]<<", "<<orthoMat[2][0]<<", "<<orthoMat[3][0]); 
            ssLOG_LINE("orthoMat: "<<orthoMat[0][1]<<", "<<orthoMat[1][1]<<", "<<orthoMat[2][1]<<", "<<orthoMat[3][1]); 
            ssLOG_LINE("orthoMat: "<<orthoMat[0][2]<<", "<<orthoMat[1][2]<<", "<<orthoMat[2][2]<<", "<<orthoMat[3][2]); 
            ssLOG_LINE("orthoMat: "<<orthoMat[0][3]<<", "<<orthoMat[1][3]<<", "<<orthoMat[2][3]<<", "<<orthoMat[3][3]); 
        
        }
        //ssLOG_LINE("orthoMat:")
        
        return orthoMat;
    }
    
    void OpenGL3_3_Common::SaveState()
    {
        if(CurrentMainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }

        if(!CurrentMainWindow->SetGLContext())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set GL state");
            return;
        }

        GL_CHECK_ERROR( glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS); );
        GL_CHECK_ERROR( glPushAttrib(GL_ALL_ATTRIB_BITS); );
        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW); );
        GL_CHECK_ERROR( glPushMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_PROJECTION); );
        GL_CHECK_ERROR( glPushMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_TEXTURE); );
        GL_CHECK_ERROR( glPushMatrix(); );
    }

    void OpenGL3_3_Common::RestoreState()
    {
        if(CurrentMainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }

        if(!CurrentMainWindow->SetGLContext())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set GL state");
            return;
        }

        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW); );
        GL_CHECK_ERROR( glPopMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_PROJECTION); );
        GL_CHECK_ERROR( glPopMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_TEXTURE); );
        GL_CHECK_ERROR( glPopMatrix(); );
        GL_CHECK_ERROR( glPopClientAttrib(); );
        GL_CHECK_ERROR( glPopAttrib(); );
    }
    
    bool OpenGL3_3_Common::DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities)
    {
        //Check if the main window is already closed
        if(CurrentMainWindow->IsClosed())
            return false;

        //glm::mat4x4 orthMat = UpdateViewPortAndModelView(CurrentMainWindow->GetRenderSize());
        //(void)orthMat;

        //Start drawing
        for(int i = 0; i < entities.size(); i++)
        {
            bool result = true;
            //Drawing text
            if(entities[i].BackendFont != nullptr)
            {
                result = DrawShape( entities[i].Vertices, 
                                    entities[i].TexCoords, 
                                    entities[i].Colors, 
                                    entities[i].Character, 
                                    *entities[i].BackendFont, 
                                    entities[i].CharacterSize);
            }
            //Drawing image
            else if(entities[i].BackendImage != nullptr)
            {
                result = DrawShape( entities[i].Vertices, 
                                    entities[i].TexCoords, 
                                    entities[i].Colors,
                                    *entities[i].BackendImage);
            }
            //Drawing shapes
            else 
            {
                result = DrawShape( entities[i].Vertices, 
                                    entities[i].Colors);
            }

            if(!result)
                return false;
        }

        return true;
    }
    
    bool OpenGL3_3_Common::AddFontCache(OpenGL3_3_Common::CharTextureIdentifier charTexture)
    {
        if(!std::get<0>(charTexture)->IsValid())
            return false;

        if(MappedFontIds.find(charTexture) != MappedFontIds.end())
            return true;

        auto& rawFont = const_cast<ssGUI::Backend::BackendFontInterface&>(*std::get<0>(charTexture));

        ssGUI::ImageData charImgData;
        if(!rawFont.GetCharacterImage(std::get<2>(charTexture), std::get<1>(charTexture), charImgData))
            return false;

        glm::ivec2 imgSize = charImgData.GetSize();

        //Convert it to rgba32
        auto rgba32Img = std::unique_ptr<uint8_t[]>(new uint8_t[imgSize.x * imgSize.y * 4]);
        ssGUI::ImageFormat format;
        void* rawPixel = charImgData.GetPixelPtr(format);
        
        if(!ssGUI::ImageUtil::ConvertToRGBA32(static_cast<void*>(rgba32Img.get()), rawPixel, format, imgSize))
            return false;

        if(!AddDrawingCache(MappedFontIds, charTexture, imgSize, rgba32Img.get()))
            return false;

        return true;
    }
    
    bool OpenGL3_3_Common::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        if(!backendImage->IsValid())
            return false;
    
        if(MappedImgIds.find(backendImage) != MappedImgIds.end())
            return true;
    
        //Get image
        ssGUI::ImageFormat format;
        const void* rawPtr =  backendImage->GetPixelPtr(format);

        //Convert it to rgba32
        auto rgba32Img = std::unique_ptr<uint8_t[]>(new uint8_t[backendImage->GetSize().x * backendImage->GetSize().y * 4]);
        if(!ssGUI::ImageUtil::ConvertToRGBA32(static_cast<void*>(rgba32Img.get()), rawPtr, format, backendImage->GetSize()))
            return false;

        if(!AddDrawingCache(MappedImgIds, backendImage, backendImage->GetSize(), rgba32Img.get()))
            return false;
    
        backendImage->Internal_AddBackendDrawingRecord(CurrentDrawingBackend);
        return true;
    }
    
    void OpenGL3_3_Common::RemoveFontCache(CharTextureIdentifier charTexture)
    {
        if(MappedFontIds.find(charTexture) == MappedFontIds.end())
            return;
            
        MappedFontIds.erase(charTexture);
    }
    
    void OpenGL3_3_Common::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        if(MappedImgIds.find(backendImage) == MappedImgIds.end())
            return;

        MappedImgIds.erase(backendImage);
        backendImage->Internal_RemoveBackendDrawingRecord(CurrentDrawingBackend);
    }
    
    void OpenGL3_3_Common::DrawToBackBuffer()
    {
        if(Vertices.empty())
            return;
    
        SaveLastViewport();
        
        SaveState();
        
        GL_CHECK_ERROR( glUseProgram(ProgramId) );
        
        glm::mat4x4 orthMat = UpdateViewPortAndModelView(CurrentMainWindow->GetWindowSize());
        
        GL_CHECK_ERROR( glBindVertexArray(VAO) );
        
        GL_CHECK_ERROR( glUniformMatrix4fv(glGetUniformLocation(ProgramId, "projMatrix"), 1, GL_FALSE, &orthMat[0][0]) );
        GL_CHECK_ERROR( glUniform1i(glGetUniformLocation(ProgramId, "ourTexture"), 0) );
        
        //NOTE: I could use  glGenSamplers and glSamplerParameteri but 1 sampler is good enough for now
        GL_CHECK_ERROR( glActiveTexture(GL_TEXTURE0) );
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, CachedImages) );
        
        GL_CHECK_ERROR( glEnable(GL_PRIMITIVE_RESTART) );
        GL_CHECK_ERROR( glPrimitiveRestartIndex(PRIMITIVE_RESTART) );
        
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, VertsVBO) );
        GL_CHECK_ERROR( glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), Vertices.data(), GL_DYNAMIC_DRAW) );
        
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, ColorsVBO) );
        GL_CHECK_ERROR( glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::u8vec4), Colors.data(), GL_DYNAMIC_DRAW) );

        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, TexCoordsVBO) );
        GL_CHECK_ERROR( glBufferData(GL_ARRAY_BUFFER, TexCoords.size() * sizeof(glm::vec3), TexCoords.data(), GL_DYNAMIC_DRAW) );

        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, TopLeftTexCoordsVBO) );
        GL_CHECK_ERROR( glBufferData(GL_ARRAY_BUFFER, TextureTopLeftCoords.size() * sizeof(glm::vec3), TextureTopLeftCoords.data(), GL_DYNAMIC_DRAW) );

        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, BotRightTexCoordsVBO) );
        GL_CHECK_ERROR( glBufferData(GL_ARRAY_BUFFER, TextureBotRightCoords.size() * sizeof(glm::vec3), TextureBotRightCoords.data(), GL_DYNAMIC_DRAW) );

        ssLOG_LINE("Vertices: "<<Vertices);
        ssLOG_LINE("Colors: "<<Colors);
        ssLOG_LINE("Idx: "<<Idx);

        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        //GL_CHECK_ERROR( glDrawElements( GL_TRIANGLES, Idx.size(), GL_UNSIGNED_INT, Idx.data()) );
        GL_CHECK_ERROR( glDrawElements( GL_TRIANGLE_FAN, Idx.size(), GL_UNSIGNED_INT, Idx.data()) );

        //GL_CHECK_ERROR( glFlush() );

        GL_CHECK_ERROR( glBindVertexArray(0) );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, 0) );
        GL_CHECK_ERROR( glUseProgram(0) );
        
        //Vertices.clear();
        //Colors.clear();
        //TexCoords.clear();
        //TextureTopLeftCoords.clear();
        //TextureBotRightCoords.clear();
        //Idx.clear();
        
        RestoreState();
        
        LoadLastViewport();
    }
    
    //void OpenGL3_3_Common::ClearBackBuffer(glm::u8vec3 clearColor)
    //{
    //    GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );
        
    //    GL_CHECK_ERROR( glClearColor(   static_cast<float>(clearColor.r) / 255.f, 
    //                                    static_cast<float>(clearColor.g) / 255.f, 
    //                                    static_cast<float>(clearColor.b) / 255.f, 
    //                                    255.f); );
    //}
    
    void* OpenGL3_3_Common::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        //TODO
    
        #if 0
        if(ImageTextures.find(backendImage) == ImageTextures.end())
            return nullptr;
        
        return &ImageTextures[backendImage];
        #endif
        return nullptr;
    }
}

}

