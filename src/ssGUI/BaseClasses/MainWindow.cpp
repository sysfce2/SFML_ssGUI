#include "ssGUI/BaseClasses/MainWindow.hpp"


namespace ssGUI
{
    MainWindow::MainWindow(MainWindow const& other) : Window(other)
    {
        BackendMainWindow = other.BackendMainWindow->Clone();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();

        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
    }
    
    MainWindow::MainWindow() : BackendMainWindow(), BackendDrawing(), LastSyncTime(0)
    {
        BackendMainWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        
        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
    }

    MainWindow::~MainWindow()
    {
        delete BackendMainWindow;
        delete BackendDrawing;
    }
    
    void MainWindow::Render()
    {
        BackendDrawing->Render(GetBackgroundColour());
    }

    ssGUI::Backend::BackendMainWindowInterface* MainWindow::GetBackendWindowInterface()
    {
        return BackendMainWindow;
    }

    ssGUI::Backend::BackendDrawingInterface* MainWindow::GetBackendDrawingInterface()
    {
        return BackendDrawing;
    }
    
    bool MainWindow::IsFocused() const
    {
        return BackendMainWindow->IsFocused();
    }
    
    void MainWindow::SetFocus(bool focus)
    {
        BackendMainWindow->SetFocus(focus);
    }

    glm::ivec2 MainWindow::GetPositionOffset()
    {
        return BackendMainWindow->GetPositionOffset();
    }

    void MainWindow::Draw()
    {
        Draw(BackendDrawing, this, glm::ivec2());
    }

    //TODO : Refactor this, merge it to sync in update function
    void MainWindow::Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {       
        //Settings that require window to be relaunched -----------------------------------------
        if(BackendMainWindow->HasTitlebar() != HasTitlebar())
            BackendMainWindow->SetTitlebar(HasTitlebar());

        bool isResizable = GetResizeType() == ssGUI::Enums::ResizeType::ALL;

        if(BackendMainWindow->IsResizable() != isResizable)
            BackendMainWindow->SetResizable(isResizable);

        if(BackendMainWindow->HasCloseButton() != IsClosable())
            BackendMainWindow->SetCloseButton(IsClosable());

        
        //Realtime settings -----------------------------------------

        if(BackendMainWindow->IsVisible() != IsVisible())
            BackendMainWindow->SetVisible(IsVisible());

        //TEST
        /*
        std::vector<glm::ivec2> cord;
        std::vector<glm::u8vec4> color;

        
        cord.push_back(glm::ivec2(1, 0));
        cord.push_back(glm::ivec2(GetSize().x - 1, 0));
        cord.push_back(glm::ivec2(GetSize().x - 1, 20));
        cord.push_back(glm::ivec2(1, 20));

        color.push_back(glm::u8vec4(0, 0, 0, 255));
        color.push_back(glm::u8vec4(0, 0, 0, 255));
        color.push_back(glm::u8vec4(0, 0, 0, 255));
        color.push_back(glm::u8vec4(0, 0, 0, 255));

        drawingInterface.DrawShape(cord, color);
        */

    }   

    glm::ivec2 MainWindow::GetPositon() const
    {
        return BackendMainWindow->GetPosition();
    }
    void MainWindow::SetPosition(glm::ivec2 position)
    {
        BackendMainWindow->SetPosition(position);
    }

    glm::ivec2 MainWindow::GetGlobalPosition()
    {
        return BackendMainWindow->GetPosition();
    }
    void MainWindow::SetGlobalPosition(glm::ivec2 position)
    {
        BackendMainWindow->SetPosition(position);
    }

    glm::ivec2 MainWindow::GetSize() const
    {
        return BackendMainWindow->GetSize();
    }
    void MainWindow::SetSize(glm::ivec2 size)
    {
        size.x = size.x > GetMaxSize().x ? GetMaxSize().x : size.x;
        size.y = size.y > GetMaxSize().y ? GetMaxSize().y : size.y;
        size.x = size.x < GetMinSize().x ? GetMinSize().x : size.x;
        size.y = size.y < GetMinSize().y ? GetMinSize().y : size.y;
        
        BackendMainWindow->SetSize(size);
    }

    ssGUI::Enums::GUIObjectType MainWindow::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::MAIN_WINDOW | ssGUI::Enums::GUIObjectType::WINDOW;
    }

    void MainWindow::Close()
    {
        //Forwarding signal to backend
        BackendMainWindow->Close();
    }

    void MainWindow::AbortClosing()
    {
        //Tell window to stop closing
        Window::AbortClosing();
        
        //Tell backend to stop closing
        BackendMainWindow->AbortClosing();
    } 

    void MainWindow::Internal_OnClose()
    {
        //Forwarding signal to window
        Window::Internal_OnClose();
    }

    void MainWindow::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus)
    {
        for(auto extension : Extensions)
            extension->Update(true, inputInterface, globalInputStatus, windowInputStatus);

        //Update cursor position offset every .5 seconds
        if(inputInterface->GetElapsedTime() - LastSyncTime > 500)
        {
            BackendMainWindow->SyncPositionOffset();
            LastSyncTime = inputInterface->GetElapsedTime();

            glm::ivec2 currentSize = BackendMainWindow->GetSize();
            
            if(currentSize.x < GetMinSize().x)
                currentSize.x = GetMinSize().x;        

            if(currentSize.y < GetMinSize().y)
                currentSize.y = GetMinSize().y;

            if(currentSize.x > GetMaxSize().x)
                currentSize.x = GetMaxSize().x;

            if(currentSize.y > GetMaxSize().x)
                currentSize.y = GetMaxSize().x;

            if(currentSize != BackendMainWindow->GetSize())
                BackendMainWindow->SetSize(currentSize);
        }


        // glm::ivec2 currentMousePos = inputInterface.GetCurrentMousePosition();
        // std::cout << "current mouse pos: "<<currentMousePos.x <<", "<<currentMousePos.y<<"\n";
        // std::cout << "current window pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout << "current window size: "<<GetSize().x<<", "<<GetSize().y<<"\n";
        // std::cout << "window offset: "<<GetPositionOffset().x << ", "<<GetPositionOffset().y<<"\n";
        // std::cout<<"\n";

        for(auto extension : Extensions)
            extension->Update(false, inputInterface, globalInputStatus, windowInputStatus);
    }

    GUIObject* MainWindow::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        MainWindow* temp = new MainWindow(*this);

        for(auto extension : Extensions)
        {
            if(!temp->IsExtensionExist(extension->GetExtensionName()))
                temp->AddExtension(extension->Clone(this));
        }

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

            if(!temp->IsEventCallbackExist(eventCallback->GetEventCallbackName()))
                temp->AddEventCallback(eventCallback->Clone(this, originalObjs, tempVec));
        }

        return temp;
    }
}