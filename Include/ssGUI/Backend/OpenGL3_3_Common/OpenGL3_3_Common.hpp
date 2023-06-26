#ifndef SSGUI_OPENGL_3_3_COMMON_HPP
#define SSGUI_OPENGL_3_3_COMMON_HPP

#include "ssGUI/Backend/DynamicImageAtlas.hpp"

#include "glad/glad.h"
#include <map>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace ssGUI
{

class DrawingEntity;

namespace Backend
{
    class BackendMainWindowInterface;
    class BackendFontInterface;
    class BackendImageInterface;
    class BackendDrawingInterface;
    
    class OpenGL3_3_Common
    {
        private:
            static const std::string VertShader;
            static const std::string FragShader;
            GLuint ProgramId;
            GLuint CachedImages;
            GLuint VAO;
            GLuint EBO;
            GLuint VertsVBO;
            GLuint ColorsVBO;
            GLuint UVsVBO;
            GLuint UseUVsVBO;

            GLint LastViewportProperties[4];

            
            std::vector<glm::vec2> Vertices;
            std::vector<glm::u8vec4> Colors;
            std::vector<glm::vec2> UVs;
            std::vector<GLint> UseUVs;
            std::vector<GLuint> Counts;


            BackendMainWindowInterface* CurrentMainWindow;
            BackendDrawingInterface* CurrentDrawingBackend;
            
            glm::ivec2 LastMainWindowSize;
            
            DynamicImageAtlas* CurrentImageAtlas;
            std::unordered_map<ssGUI::Backend::BackendImageInterface*, int> MappedImgIds;

            //TODO: Method to generate mipmap

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize);
                            
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image);

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors);
            
            void DrawShapesToBackBuffer();
            
            bool OnNewAtlasRequest();
            
            void SaveLastViewport();
            
            void LoadLastViewport();
    
        public:
            using CharSize = uint16_t;
            using CharCode = uint32_t;
            using CharTextureIdentifier = std::tuple<ssGUI::Backend::BackendFontInterface*, CharSize, CharCode>;
        
            OpenGL3_3_Common(BackendMainWindowInterface* mainWindow, BackendDrawingInterface* drawingBackend);
            
            ~OpenGL3_3_Common();
        
            glm::mat4x4 UpdateViewPortAndModelView(glm::ivec2 widthHeight);
            void SaveState();
            void RestoreState();
            bool DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities);
            void AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage);
            void RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage);
            
            //TODO: May not be able to implement
            void* GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage);
    };

}

}

#endif