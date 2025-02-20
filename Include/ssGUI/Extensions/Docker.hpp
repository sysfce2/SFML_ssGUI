#ifndef SSGUI_DOCKER_H
#define SSGUI_DOCKER_H

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/ssGUITags.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace ssGUI 
{ 
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    class Layout;
    
    //class: ssGUI::Extensions::Docker
    /* This extensions can either be added explictly or generated automatically.
    
    In the case of being generated Automactically:
    This extension allows this GUI object to acts as an conatiner for grouping the docked windows together.
    An empty window (with <Layout> and <Docker> attached) is automatically created (or destroyed) if needed for grouping docked windows.

    In the case of being added explictly:
    This extension allows windows with <Dockable> extension attached to be docked to this GUI object.
    This GUI object does not have to be a window.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;                                //See <BindToObject>
        bool Enabled;                                               //See <IsEnabled>
        bool ChildrenDockerUseThisSettings;                         //See <IsChildrenDockerUseThisSettings>
                                                                    //This works by having the child checking if parent has docker or not. 
                                                                    //Therefore if the docker extension is added after the child being added, this won't work.

        bool UseTriggerPercentage;                                  //See <IsUseTriggerPercentage>
        float TriggerHorizontalPercentage;                          //See <GetTriggerHorizontalPercentage>
        float TriggerVerticalPercentage;                            //See <GetTriggerVerticalPercentage>
        int TriggerHorizontalPixel;                                 //See <GetTriggerHorizontalPixel>
        int TriggerVerticalPixel;                                   //See <GetTriggerVerticalPixel>
        glm::u8vec4 TriggerAreaColor;                               //See <GetTriggerAreaColor>
        glm::u8vec4 DockPreviewColor;                               //See <GetDockPreviewColor>

        ssGUI::GUIObject* DockPreivew;                              //(Internal variable) Dock preview GUI visual object
        ssGUI::GUIObject* DockTrigger;                              //(Internal variable) Dock trigger GUI visual object

        bool ChildRemoveGuard;                                      //(Internal variable) Used to prevent listener triggering itself
        bool ValidDocking;                                          //See <IsValidDocking>

        static ssGUI::Window* DefaultGeneratedDockerWindow;         //See <GetDefaultGeneratedDockerWindow>
    =================================================================
    ============================== C++ ==============================
    Docker::Docker() :  Container(nullptr),
                        Enabled(true),
                        ChildrenDockerUseThisSettings(true),
                        UseTriggerPercentage(true),
                        TriggerHorizontalPercentage(0.5),
                        TriggerVerticalPercentage(0.5),
                        TriggerHorizontalPixel(15),
                        TriggerVerticalPixel(15),
                        TriggerAreaColor(87, 207, 255, 127),
                        DockPreviewColor(255, 255, 255, 127),
                        DockPreivew(nullptr),
                        DockTrigger(nullptr),
                        ChildRemoveGuard(false)
    {}

    ssGUI::Window* Docker::DefaultGeneratedDockerWindow = nullptr;
    =================================================================
    */
    class Docker : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            Docker& operator=(Docker const& other);

        protected:
            ssGUI::GUIObject* Container;                                //See <BindToObject>
            bool Enabled;                                               //See <IsEnabled>
            bool ChildrenDockerUseThisSettings;                         //See <IsChildrenDockerUseThisSettings>
                                                                        //This works by having the child checking if parent has docker or not. 
                                                                        //Therefore if the docker extension is added after the child being added, this won't work.

            bool UseTriggerPercentage;                                  //See <IsUseTriggerPercentage>
            float TriggerHorizontalPercentage;                          //See <GetTriggerHorizontalPercentage>
            float TriggerVerticalPercentage;                            //See <GetTriggerVerticalPercentage>
            int TriggerHorizontalPixel;                                 //See <GetTriggerHorizontalPixel>
            int TriggerVerticalPixel;                                   //See <GetTriggerVerticalPixel>
            glm::u8vec4 TriggerAreaColor;                               //See <GetTriggerAreaColor>
            glm::u8vec4 DockPreviewColor;                               //See <GetDockPreviewColor>

            ssGUI::GUIObject* DockPreivew;                              //(Internal variable) Dock preview GUI visual object
            ssGUI::GUIObject* DockTrigger;                              //(Internal variable) Dock trigger GUI visual object

            bool ChildRemoveGuard;                                      //(Internal variable) Used to prevent listener triggering itself
            bool ValidDocking;                                          //See <IsValidDocking>

            static ssGUI::Window* DefaultGeneratedDockerWindow;         //See <GetDefaultGeneratedDockerWindow>

            Docker();
            virtual ~Docker() override;
            Docker(Docker const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

            virtual void CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color);
            virtual void DrawPreview();
            virtual void DiscardPreview();
            virtual void DrawTriggerArea();
            virtual void DiscardTriggerArea();
            virtual int GetRealChildrenCount(ssGUI::GUIObject* checkObj);

            virtual void ChildRemoved(ssGUI::GUIObject* child);

        public:
            static const std::string EXTENSION_NAME;
 
            /*function: SetDefaultGeneratedFloatingDockerWindow
            Sets the default generated docker window. Set it to nullptr if you want to stop using the one passed it earlier.
            Do not pass in deleted or invalid pointer.
            The window will look as it is when floating, but invisible when not floating.*/
            static void SetDefaultGeneratedFloatingDockerWindow(ssGUI::Window* window);

            //function: GetDefaultGeneratedFloatingDockerWindow
            //Returns the default generated docker window. Returns nullptr if it is not set and will just use <ssGUI::Window>. 
            static ssGUI::Window* GetDefaultGeneratedFloatingDockerWindow();

            //function: SetChildrenDockerUseThisSettings
            //If true, this will set any docker to use this settings *when it is added as a child*
            virtual void SetChildrenDockerUseThisSettings(bool use);

            //function: IsChildrenDockerUseThisSettings
            //If true, this will set any docker to use this settings *when it is added as a child*
            virtual bool IsChildrenDockerUseThisSettings() const;

            //function: SetUseTriggerPercentage
            virtual void SetUseTriggerPercentage(bool use);

            //function: IsUseTriggerPercentage
            virtual bool IsUseTriggerPercentage() const;

            //function: SetTriggerHorizontalPercentage
            virtual void SetTriggerHorizontalPercentage(float percent);

            //function: GetTriggerHorizontalPercentage
            virtual float GetTriggerHorizontalPercentage() const;

            //function: SetTriggerVerticalPercentage
            virtual void SetTriggerVerticalPercentage(float percent);

            //function: GetTriggerVerticalPercentage
            virtual float GetTriggerVerticalPercentage() const;

            //function: SetTriggerHorizontalPixel
            virtual void SetTriggerHorizontalPixel(int pixel);

            //function: GetTriggerHorizontalPixel
            virtual int GetTriggerHorizontalPixel() const;

            //function: SetTriggerVerticalPixel
            virtual void SetTriggerVerticalPixel(int pixel);

            //function: GetTriggerVerticalPixel
            virtual int GetTriggerVerticalPixel() const;

            //function: SetTriggerAreaColor
            virtual void SetTriggerAreaColor(glm::u8vec4 color);

            //function: GetTriggerAreaColor 
            virtual glm::u8vec4 GetTriggerAreaColor() const;

            //function: SetDockPreviewColor
            virtual void SetDockPreviewColor(glm::u8vec4 color);

            //function: GetDockPreviewColor
            virtual glm::u8vec4 GetDockPreviewColor() const;

            //function: IsValidDocking
            virtual bool IsValidDocking() const;
 
            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;
            
            //function: Clone
            //See <Extension::Clone>
            virtual Docker* Clone() override;
    };
}

}

#endif