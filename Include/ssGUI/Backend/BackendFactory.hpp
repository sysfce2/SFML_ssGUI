#ifndef SSGUI_BACKEND_FACTORY_H
#define SSGUI_BACKEND_FACTORY_H

//TODO: Move method implementation to .cpp

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

#ifdef SSGUI_MAIN_BACKEND_SFML
    #include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendFontSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendSystemInputSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#endif

#ifdef SSGUI_FONT_BACKEND_FREE_TYPE
    #include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"
#endif

#ifdef SSGUI_MAIN_BACKEND_WIN32_OPENGL
    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"
    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"
#endif

#ifdef SSGUI_MAIN_BACKEND_X11_OPENGL
    #include "ssGUI/Backend/X11_OpenGL3_3/BackendMainWindowX11_OpenGL3_3.hpp"
    #include "ssGUI/Backend/X11_OpenGL3_3/BackendDrawingX11_OpenGL3_3.hpp"
    #include "ssGUI/Backend/X11_OpenGL3_3/BackendSystemInputX11_OpenGL3_3.hpp"
#endif

#ifdef SSGUI_FONT_BACKEND_FREE_TYPE
    #include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"
#endif

#ifdef SSGUI_IMAGE_BACKEND_STB_IMAGE
    #include "ssGUI/Backend/stb_image/BackendImageStbImage.hpp"
#endif

#ifdef SSGUI_MOCK_BACKEND
    #include "ssGUI/Backend/Mocks/BackendDrawingMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendImageMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendSystemInputMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendMainWindowMock.hpp"
#endif

namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFactory
    //BackendFactory is used to create backend objects without referencing the actual backend by using interfaces.
    class BackendFactory
    {
        //TODO: Add exit code or log when no backends are found
        public:
            //function: CreateBackendDrawingInterface
            static ssGUI::Backend::BackendDrawingInterface* CreateBackendDrawingInterface()
            {
                ssGUI::Backend::BackendDrawingInterface* returnInterface = nullptr;
                
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    returnInterface = static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingSFML());
                #elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
                    returnInterface = static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingWin32_OpenGL3_3());
                #elif defined SSGUI_MAIN_BACKEND_X11_OPENGL
                    returnInterface =  static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingX11_OpenGL3_3());
                #else
                    returnInterface = nullptr;
                #endif

                #ifdef SSGUI_MOCK_BACKEND
                    returnInterface = static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingMock(returnInterface));
                #else
                    if(returnInterface == nullptr)
                    {
                        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
                        ssLOG_EXIT_PROGRAM();
                    }
                #endif

                return returnInterface;
            }

            //function: CreateBackendFontInterface
            static ssGUI::Backend::BackendFontInterface* CreateBackendFontInterface()
            {
                ssGUI::Backend::BackendFontInterface* returnInterface = nullptr;
                
                #ifdef SSGUI_FONT_BACKEND_SFML
                    returnInterface = static_cast<ssGUI::Backend::BackendFontInterface*>(new ssGUI::Backend::BackendFontSFML());
                #elif defined SSGUI_FONT_BACKEND_FREE_TYPE
                    returnInterface = static_cast<ssGUI::Backend::BackendFontInterface*>(new ssGUI::Backend::BackendFontFreeType());
                #else
                    returnInterface = nullptr;
                #endif
                
                #ifdef SSGUI_MOCK_BACKEND
                    returnInterface = static_cast<ssGUI::Backend::BackendFontInterface*>(new ssGUI::Backend::BackendFontMock(returnInterface));
                #else
                    if(returnInterface == nullptr)
                    {
                        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
                        ssLOG_EXIT_PROGRAM();
                    }
                #endif
                
                return returnInterface;
            }

            //function: CreateBackendImageInterface
            static ssGUI::Backend::BackendImageInterface* CreateBackendImageInterface()
            {
                ssGUI::Backend::BackendImageInterface* returnInterface = nullptr;

                #ifdef SSGUI_IMAGE_BACKEND_SFML
                    returnInterface = static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageSFML());
                #elif defined SSGUI_IMAGE_BACKEND_STB_IMAGE
                    returnInterface = static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageStbImage());
                #else
                    returnInterface = nullptr;
                #endif
                
                #ifdef SSGUI_MOCK_BACKEND
                    returnInterface = static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageMock(returnInterface));
                #else
                    if(returnInterface == nullptr)
                    {
                        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
                        ssLOG_EXIT_PROGRAM();
                    }
                #endif
                
                return returnInterface;
            }

            //function: CreateBackendInputInterface
            static ssGUI::Backend::BackendSystemInputInterface* CreateBackendInputInterface()
            {
                ssGUI::Backend::BackendSystemInputInterface* returnInterface = nullptr;
                
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    returnInterface = static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputSFML());
                #elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
                    returnInterface = static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3());
                #elif defined SSGUI_MAIN_BACKEND_X11_OPENGL
                    returnInterface = static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputX11_OpenGL3_3());
                #else
                    returnInterface = nullptr;
                #endif
                
                #ifdef SSGUI_MOCK_BACKEND
                    returnInterface = static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputMock(returnInterface));
                #else
                    if(returnInterface == nullptr)
                    {
                        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
                        ssLOG_EXIT_PROGRAM();
                    }
                #endif
                
                return returnInterface;
            }
            
            //function: CreateBackendMainWindowInterface
            static ssGUI::Backend::BackendMainWindowInterface* CreateBackendMainWindowInterface()
            {
                ssGUI::Backend::BackendMainWindowInterface* returnInterface = nullptr;

                #ifdef SSGUI_MAIN_BACKEND_SFML
                    returnInterface = static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowSFML());
                #elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
                    returnInterface = static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowWin32_OpenGL3_3());
                #elif defined SSGUI_MAIN_BACKEND_X11_OPENGL
                    returnInterface = static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowX11_OpenGL3_3());
                #else
                    returnInterface = nullptr;
                #endif
                
                #ifdef SSGUI_MOCK_BACKEND
                    returnInterface = static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowMock(returnInterface));
                #else
                    if(returnInterface == nullptr)
                    {
                        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
                        ssLOG_EXIT_PROGRAM();
                    }
                #endif
                
                return returnInterface;
            }
    };
}

}

#endif