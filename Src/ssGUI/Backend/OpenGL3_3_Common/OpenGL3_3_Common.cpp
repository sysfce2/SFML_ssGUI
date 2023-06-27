#include "ssGUI/Backend/OpenGL3_3_Common/OpenGL3_3_Common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/HelperClasses/ImageUtil.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include "ssGUI/DataClasses/DrawingEntity.hpp"

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

namespace ssGUI
{

namespace Backend
{
    const std::string OpenGL3_3_Common::VertShader = 
    R"(
        #version 330 core
        
        in vec3 vertPos;
        in vec4 vertColor;
        in vec3 vertTexCoord;
        in int vertIsUV;
        in vec2 vertTexTopLeft;
        in vec2 vertTexBotRight;

        out vec4 fragColor;
        flat out int fragIsUV;
        out vec3 fragTexCoord;
        out vec2 fragTexTopLeft;
        out vec2 fragTexBotRight;

        uniform mat4 projMatrix;

        void main()
        {
            gl_Position = vec4(vertPos, 1.0);
            gl_Position = projMatrix * gl_Position;
            
            fragColor = vertColor;
            fragIsUV = vertIsUV;
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
        flat in int fragIsUV;
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
            if(fragIsUV > 0)
            {
                outColor = fragColor;
                return;
            }
            
            float mipmapLevel = 0;
        
            #ifdef GL_ARB_texture_query_lod
                mipmapLevel = textureQueryLOD(ourTexture, vec2(fragTexCoord.x, fragTexCoord.y)).x;
            #else
                mipmapLevel = mip_map_level(fragTexCoord.xy * textureSize(ourTexture, 0).xy);
            #endif
            
            int roundedMipmapLevel = int(mipmapLevel + 0.5f);
            
            //If mipmap level is 0, don't need to do anything
            if(roundedMipmapLevel == 0)
            {
                outColor = texture(ourTexture, fragTexCoord) * fragColor;
                return;
            }
            
            //Otherwise get the localized UV
            vec2 localTexCoord = (fragTexCoord.xy - fragTexTopLeft) / (fragTexBotRight - fragTexTopLeft);
            
            //And multiply by 0.5 ^ mipmap level
            float mipmapMultiplier = pow(0.5f, float(roundedMipmapLevel));
            
            //The coordinate of the mipmap in y can be found by using the nth partial sum of a geometric sequence
            //  with this (modified) formula: (1 - 0.5 ^ mipmapLevel) / (1 - 0.5) - 1
            float mipmapYOffsetMultiplier = (1 - mipmapMultiplier) / 0.5f - 1;
            
            vec3 mipmapTexCoord = vec3( fragTexBotRight.x + localTexCoord.x * mipmapMultiplier, 
                                        fragTexTopLeft.y + mipmapYOffsetMultiplier + localTexCoord.y * mipmapMultiplier,
                                        fragTexCoord.z);
        
            outColor = texture(ourTexture, mipmapTexCoord) * fragColor;
        }  
    )";


    bool OpenGL3_3_Common::DrawShape(const std::vector<glm::vec2>& vertices, 
                                                const std::vector<glm::vec2>& texCoords,
                                                const std::vector<glm::u8vec4>& colors,
                                                const uint32_t character,
                                                const ssGUI::Backend::BackendFontInterface& font,
                                                int characterSize)
    {
        #if 0
        if(!font.IsValid())
            return false;
    
        auto& rawFont = const_cast<ssGUI::Backend::BackendFontInterface&>(font);

        ssGUI::ImageData charImgData;
        if(!rawFont.GetCharacterImage(character, characterSize, charImgData))
            return false;

        CurrentMainWindow->SetGLContext();
    
        glm::ivec2 imgSize = charImgData.GetSize();
        CharTextureIdentifier curIdentifier = CharTextureIdentifier(&rawFont, characterSize, character);

        if(CharTextures.find(curIdentifier) == CharTextures.end())
        {
            GLuint textureId = 0;

            GL_CHECK_ERROR( glGenTextures(1, &textureId); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );

            //Convert it to rgba32
            uint8_t* rgba32Img = new uint8_t[charImgData.GetSize().x * charImgData.GetSize().y * 4];
            ssGUI::ImageFormat format;
            void* rawPixel = charImgData.GetPixelPtr(format);
            
            if(!ssGUI::ImageUtil::ConvertToRGBA32(static_cast<void*>(rgba32Img), rawPixel, format, imgSize))
            {
                delete[] rgba32Img;
                GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
                return false;
            }
            
            GL_CHECK_ERROR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSize.x, imgSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                            rgba32Img); );

            CharTextures[curIdentifier] = textureId;

            delete[] rgba32Img;
        }
        else
        {
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, CharTextures[curIdentifier]); );
        }
         
        GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D); );
        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0; i < vertices.size(); i++)
        {
            glm::vec2 texCoord = texCoords[i];

            texCoord.x /= imgSize.x;
            texCoord.y /= imgSize.y;
            
            glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            glTexCoord2f(texCoord.x, texCoord.y);
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            glVertex3f(round(vertices[i].x), round(vertices[i].y), 0);
        }

        GL_CHECK_ERROR( glEnd(); );
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
        GL_CHECK_ERROR( glFlush(); );
        #endif
        
        return true;        
    }

    bool OpenGL3_3_Common::DrawShape(const std::vector<glm::vec2>& vertices, 
                                                const std::vector<glm::vec2>& texCoords,
                                                const std::vector<glm::u8vec4>& colors,
                                                const ssGUI::Backend::BackendImageInterface& image)
    {
        ssLOG_LINE();
    
        #if 1
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
            
            //TODO: Remove const cast
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
        #else
            CurrentMainWindow->SetGLContext();

            if(!image.IsValid())
                return false;

            //GL_CHECK_ERROR( glUseProgram(ProgramId); );
            
            AddImageCache(const_cast<ssGUI::Backend::BackendImageInterface*>(&image));
            
            glm::ivec2 imgSize = image.GetSize();
            
            //TODO: Remove const cast
            //If unable to add to cache, return
            //if(ImageTextures.find(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)) == ImageTextures.end())
            //    return false;
            //else
            //    GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, ImageTextures[const_cast<ssGUI::Backend::BackendImageInterface*>(&image)]); );

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
                //glTexCoord2f(texCoord.x, texCoord.y);

                //glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
                glColor4ub(255, colors[i].g, colors[i].b, colors[i].a);

                //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
                glVertex3f(round(vertices[i].x), round(vertices[i].y), 0);
            }


            GL_CHECK_ERROR( glEnd(); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
            GL_CHECK_ERROR( glFlush(); );
        #endif

        return true;    
    }


    bool OpenGL3_3_Common::DrawShape(const std::vector<glm::vec2>& vertices, 
                                                const std::vector<glm::u8vec4>& colors)
    {
        CurrentMainWindow->SetGLContext();

        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0; i < vertices.size(); i++)
        {
            glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);   
            glVertex3f(vertices[i].x, vertices[i].y, 0);
        }

        GL_CHECK_ERROR( glEnd(); );
        GL_CHECK_ERROR( glFlush(); );

        return true;
    }
    
    void OpenGL3_3_Common::DrawShapesToBackBuffer()
    {
        //TODO   
    }
    
    bool OpenGL3_3_Common::OnNewAtlasRequest()
    {
        //TODO
        return false;
    }
    
    void OpenGL3_3_Common::SaveLastViewport()
    {
        //TODO        
    }
    
    void OpenGL3_3_Common::LoadLastViewport()
    {
        //TODO        
    }
    
    OpenGL3_3_Common::OpenGL3_3_Common( BackendMainWindowInterface* mainWindow,
                                        BackendDrawingInterface* drawingBackend) :  ProgramId(0),
                                                                                    CachedImages(0),
                                                                                    CurrentMainWindow(nullptr),
                                                                                    LastMainWindowSize(),
                                                                                    CurrentImageAtlas(nullptr)
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
        GL_CHECK_ERROR( glGenBuffers(1, &EBO) );
        
        //Generate ID for VBOs for vertex pos, colors, UVs and UseUVs flag
        GL_CHECK_ERROR( glGenBuffers(1, &VertsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &ColorsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &UVsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &UseUVsVBO) );
        
        //Generate ID for VAO and bind VBOs and EBO created above to this VAO
        GL_CHECK_ERROR( glGenVertexArrays(1, &VAO) );
        
        GL_CHECK_ERROR( glBindVertexArray(VAO) );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, VertsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0) );
        
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, ColorsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0) );
        
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, UVsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0) );

        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, UseUVsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(3, 1,  GL_INT, GL_FALSE, 0, 0) );

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

        glm::mat4x4 orthMat = UpdateViewPortAndModelView(CurrentMainWindow->GetRenderSize());
        (void)orthMat;

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
    
    void OpenGL3_3_Common::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        if(!backendImage->IsValid())
            return;
    
        if(MappedImgIds.find(backendImage) != MappedImgIds.end())
            return;
            
        ssLOG_LINE();
            
        {
            //Allocate space on atlas
            DynamicImageAtlas::ImageAtlasImageInfo imgInfo;
            imgInfo.ImageSizeInPixel = backendImage->GetSize();
            imgInfo.HasMipmap = true;
            int imgId;
            
            if(!CurrentImageAtlas->RequestImage(imgInfo, imgId))
            {
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to request atlas image");
                return;
            }
            
            if(!CurrentImageAtlas->GetImageInfo(imgId, imgInfo))
            {
                ssGUI_ERROR("Internal error, ImageAtlas failed internally");
                return;
            }
            
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, CachedImages) );
            
            //Get image
            ssGUI::ImageFormat format;
            const void* rawPtr = backendImage->GetPixelPtr(format);

            //Convert it to rgba32
            uint8_t* rgba32Img = new uint8_t[backendImage->GetSize().x * backendImage->GetSize().y * 4];
            if(!ssGUI::ImageUtil::ConvertToRGBA32(static_cast<void*>(rgba32Img), rawPtr, format, backendImage->GetSize()))
            {
                delete[] rgba32Img;
                return;
            }
            
            //Write the image data to the texture
            ssLOG_LINE("imgInfo: "<<imgInfo);
            
            for(int i = 0; i < 5; i++)
            {
                ssLOG_LINE("rgba32Img[" << i << "]: "<<(int)rgba32Img[i * 4 + 0] <<", "<<(int)rgba32Img[i * 4 + 1] <<", "<<(int)rgba32Img[i * 4 + 2]<<", "<<(int)rgba32Img[i * 4 + 3]);
            }
            
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
                                            rgba32Img) );
                                            
            delete[] rgba32Img;
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D_ARRAY, 0) );
            
            
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
            //DEBUG Ends
            //================================================================================

            
            MappedImgIds[backendImage] = imgId;
            backendImage->Internal_AddBackendDrawingRecord(CurrentDrawingBackend);
            
            if(backendImage->GetSize().x == 1 || backendImage->GetSize().y == 1)
                return;

            glm::ivec2 lastMipmapSize = backendImage->GetSize();
            glm::ivec2 lastMipmampPixelPosOffset = glm::ivec2(0, 0);
            glm::ivec2 currentMipmampPixelPosOffset = glm::ivec2(lastMipmapSize.x, 0);
            
            const int MAX_LOOP = 100;
            const int READ_FB = 0;
            const int DRAW_FB = 1;
            
            //GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D) );
            
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
                    return;
                }
            }
            
            //GLuint textureId = 0;

            //GL_CHECK_ERROR( glGenTextures(1, &textureId); );
            //GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );
            //GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
            //GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
            //GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
            //GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );
            
            ////Save it to gpu
            //GL_CHECK_ERROR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backendImage->GetSize().x, backendImage->GetSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
            //                rgba32Img); );

            //ImageTextures[backendImage] = textureId;

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
            
            GL_CHECK_ERROR( glDeleteRenderbuffers(1, &mipmapRenderbuffer) );
            GL_CHECK_ERROR( glDeleteFramebuffers(2, readDrawFramebuffers) );

            ssLOG_LINE();

            //MappedImgIds[backendImage] = imgId;
            //backendImage->Internal_AddBackendDrawingRecord(CurrentDrawingBackend);
        }
        
        //GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
    }
    
    void OpenGL3_3_Common::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        //TODO
    
        #if 0
        if(ImageTextures.find(backendImage) == ImageTextures.end())
            return;

        ImageTextures.erase(backendImage);
        backendImage->Internal_RemoveBackendDrawingRecord(this);
        #endif
    }
    
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

