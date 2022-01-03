#include "ssGUI/BaseClasses/BaseGUIObject.hpp"

#include "ssGUI/BaseClasses/MainWindow.hpp" //This is for getting the MainWindow offset
#include "ssGUI/ssGUIManager.hpp" //This is for accessing DeletedObjs

namespace ssGUI
{    
    BaseGUIObject::BaseGUIObject(BaseGUIObject const& other)
    {
        Parent = -1;
        SetParent(other.GetParent()); //Note : Reason of using SetParent is to inform the parent to add this as a child
        Children = std::list<ssGUIObjectIndex>();
        CurrentChild = Children.end();
        CurrentChildIteratorEnd = true;
        Visible = other.IsVisible();
        BackgroundColour = other.GetBackgroundColour();
        UserCreated = other.IsUserCreated();
        ObjectDelete = other.Internal_IsDeleted();
        HeapAllocated = other.IsHeapAllocated();
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        DestroyEventCalled = other.DestroyEventCalled;
        Position = other.GetPosition();
        GlobalPosition = other.GlobalPosition;
        Size = other.GetSize();
        MinSize = other.GetMinSize();
        MaxSize = other.GetMaxSize();
        Anchor = other.GetAnchorType();
        DrawingVerticies = other.DrawingVerticies;
        DrawingUVs = other.DrawingUVs;
        DrawingColours = other.DrawingColours;
        DrawingCounts = other.DrawingCounts;
        DrawingProperties = std::vector<ssGUI::DrawingProperty>(other.DrawingProperties);
        Extensions = std::unordered_map<std::string, ssGUI::Extensions::Extension*>();
        EventCallbacks = std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*>();
        CurrentTags = other.CurrentTags;// std::unordered_set<std::string>();
    }
    
    void BaseGUIObject::SyncPosition()
    {
        FUNC_DEBUG_ENTRY();
        
        if(Parent == -1)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(CurrentObjectsReferences.GetObjectReference(Parent) == nullptr)
        {
            Parent = -1;
            FUNC_DEBUG_EXIT();
            return;
        }
        
        glm::ivec2 parentGlobalPositon = CurrentObjectsReferences.GetObjectReference(Parent)->GetParent() == nullptr ? 
                                            glm::ivec2() : CurrentObjectsReferences.GetObjectReference(Parent)->GetGlobalPosition();
        glm::ivec2 parentSize = CurrentObjectsReferences.GetObjectReference(Parent)->GetSize();
        glm::ivec2 anchorPosition = parentGlobalPositon;
        glm::ivec2 anchorDirection = glm::ivec2(1, 1);
        glm::ivec2 positionOffset = GetSize();
        
        //Find anchor position
        switch(Anchor)
        {
            case ssGUI::Enums::AnchorType::TOP_LEFT:
                anchorPosition = parentGlobalPositon;
                positionOffset = glm::ivec2(0, 0);
                break;
            case ssGUI::Enums::AnchorType::TOP_RIGHT:
                anchorPosition = parentGlobalPositon + glm::ivec2(parentSize.x, 0);
                anchorDirection.x = -1;
                positionOffset = glm::ivec2(GetSize().x, 0);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_LEFT:
                anchorPosition = parentGlobalPositon + glm::ivec2(0, parentSize.y);
                anchorDirection.y = -1;
                positionOffset = glm::ivec2(0, GetSize().y);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_RIGHT:
                anchorPosition = parentGlobalPositon + glm::ivec2(parentSize.x, parentSize.y);
                anchorDirection *= -1;
                positionOffset = glm::ivec2(GetSize().x, GetSize().y);
                break;
        }

        if(CurrentObjectsReferences.GetObjectReference(Parent)->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            CurrentObjectsReferences.GetObjectReference(Parent)->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            anchorPosition.y += dynamic_cast<ssGUI::Window*>(CurrentObjectsReferences.GetObjectReference(Parent))->GetTitlebarHeight();
        }

        //Local Position = (Global position - Anchor Point) * anchorDirection
        Position.x = (GlobalPosition.x + positionOffset.x - anchorPosition.x) * anchorDirection.x;
        Position.y = (GlobalPosition.y + positionOffset.y - anchorPosition.y) * anchorDirection.y;

        FUNC_DEBUG_EXIT();
    }
    
    void BaseGUIObject::SyncGlobalPosition()
    {
        FUNC_DEBUG_ENTRY();
        
        if(Parent == -1)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(CurrentObjectsReferences.GetObjectReference(Parent) == nullptr)
        {
            Parent = -1;
            FUNC_DEBUG_EXIT();
            return;
        }
        
        glm::ivec2 parentGlobalPositon = CurrentObjectsReferences.GetObjectReference(Parent)->GetParent() == nullptr ? glm::ivec2() : CurrentObjectsReferences.GetObjectReference(Parent)->GetGlobalPosition();
        glm::ivec2 parentSize = CurrentObjectsReferences.GetObjectReference(Parent)->GetSize();
        glm::ivec2 anchorPosition = parentGlobalPositon;
        glm::ivec2 anchorDirection = glm::ivec2(1, 1);
        glm::ivec2 positionOffset = GetSize();

        // std::cout<<"parent pos:"<<parentGlobalPositon.x<<", "<<parentGlobalPositon.y<<"\n";
        // std::cout<<"parentSize: "<<parentSize.x<<", "<<parentSize.y<<"\n";
        
        //Find anchor position
        switch(Anchor)
        {
            case ssGUI::Enums::AnchorType::TOP_LEFT:
                anchorPosition = parentGlobalPositon;
                positionOffset = glm::ivec2(0, 0);
                break;
            case ssGUI::Enums::AnchorType::TOP_RIGHT:
                anchorPosition = parentGlobalPositon + glm::ivec2(parentSize.x, 0);
                anchorDirection.x = -1;
                positionOffset = glm::ivec2(GetSize().x, 0);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_LEFT:
                anchorPosition = parentGlobalPositon + glm::ivec2(0, parentSize.y);
                anchorDirection.y = -1;
                positionOffset = glm::ivec2(0, GetSize().y);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_RIGHT:
                anchorPosition = parentGlobalPositon + glm::ivec2(parentSize.x, parentSize.y);
                anchorDirection *= -1;
                positionOffset = glm::ivec2(GetSize().x, GetSize().y);
                break;
        }

        if(CurrentObjectsReferences.GetObjectReference(Parent)->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            CurrentObjectsReferences.GetObjectReference(Parent)->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            anchorPosition.y += dynamic_cast<ssGUI::Window*>(CurrentObjectsReferences.GetObjectReference(Parent))->GetTitlebarHeight();
        }

        // std::cout<<"anchorPosition: "<<anchorPosition.x<<", "<<anchorPosition.y<<"\n";
        // std::cout<<"anchorDirection: "<<anchorDirection.x<<", "<<anchorDirection.y<<"\n";
        // std::cout<<"positionOffset: "<<positionOffset.x<<", "<<positionOffset.y<<"\n";

        //Global Position = AnchorPoint + local position * anchorDirection
        GlobalPosition.x = anchorPosition.x + Position.x * anchorDirection.x - positionOffset.x;
        GlobalPosition.y = anchorPosition.y + Position.y * anchorDirection.y - positionOffset.y;

        FUNC_DEBUG_EXIT();
    }

    void BaseGUIObject::NotifyAndRemoveOnObjectDestroyEventCallbackIfExist()
    {
        if(DestroyEventCalled)
            return;
        
        DestroyEventCalled = true;
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            GetEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME)->Notify(this);
            RemoveEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME);
        }
    }

    BaseGUIObject::BaseGUIObject() : Parent(-1), Children(), CurrentChild(Children.end()), CurrentChildIteratorEnd(true), Visible(true),
                                        BackgroundColour(glm::u8vec4(255, 255, 255, 255)), UserCreated(true), ObjectDelete(false), HeapAllocated(false),
                                        CurrentObjectsReferences(), DestroyEventCalled(false), Position(glm::ivec2(0, 0)), 
                                        GlobalPosition(glm::ivec2(0, 0)), Size(glm::ivec2(50, 50)), MinSize(glm::ivec2(25, 25)),
                                        MaxSize(glm::ivec2(std::numeric_limits<int>::max(), std::numeric_limits<int>::max())),
                                        Anchor(ssGUI::Enums::AnchorType::TOP_LEFT), DrawingVerticies(), DrawingUVs(), DrawingColours(), 
                                        DrawingCounts(), DrawingProperties(), Extensions(), EventCallbacks(), CurrentTags()
    {}

    BaseGUIObject::~BaseGUIObject()
    {
        if(!ObjectDelete)
        {
            CurrentObjectsReferences.CleanUp();
            
            NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
            
            for(auto it : Extensions)
                delete it.second;
            
            for(auto it : EventCallbacks)
                delete it.second;
        }
    }

    //Below are from GUIObject.hpp
    glm::ivec2 BaseGUIObject::GetPosition() const
    {
        return Position;
    }

    void BaseGUIObject::SetPosition(glm::ivec2 position)
    {
        Position = position;
    }

    glm::ivec2 BaseGUIObject::GetGlobalPosition()
    {
        //Update global position
        SyncGlobalPosition();
        
        return GlobalPosition;
    }

    void BaseGUIObject::SetGlobalPosition(glm::ivec2 position)
    {
        GlobalPosition = position;

        //Update local position
        SyncPosition();
    }

    glm::ivec2 BaseGUIObject::GetSize() const
    {
        return Size;
    }

    void BaseGUIObject::SetSize(glm::ivec2 size)
    {
        size.x = size.x > MaxSize.x ? MaxSize.x : size.x;
        size.y = size.y > MaxSize.y ? MaxSize.y : size.y;
        size.x = size.x < MinSize.x ? MinSize.x : size.x;
        size.y = size.y < MinSize.y ? MinSize.y : size.y;

        Size = size;
    }

    glm::ivec2 BaseGUIObject::GetMinSize() const
    {
        return MinSize;
    }

    void BaseGUIObject::SetMinSize(glm::ivec2 minSize)
    {
        MinSize = minSize;
        MaxSize.x = MinSize.x > MaxSize.x ? MinSize.x : MaxSize.x;
        MaxSize.y = MinSize.y > MaxSize.y ? MinSize.y : MaxSize.y;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME)->Notify(this);
    }

    glm::ivec2 BaseGUIObject::GetMaxSize() const
    {
        return MaxSize;
    }

    void BaseGUIObject::SetMaxSize(glm::ivec2 maxSize)
    {
        MaxSize = maxSize;
        MinSize.x = MaxSize.x < MinSize.x ? MaxSize.x : MinSize.x;
        MinSize.y = MaxSize.y < MinSize.y ? MaxSize.y : MinSize.y;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME)->Notify(this);
    }

    ssGUI::GUIObject* BaseGUIObject::GetParent() const 
    {
        return Parent == -1 ? nullptr : CurrentObjectsReferences.GetObjectReference(Parent);
    }

    void BaseGUIObject::SetParent(ssGUI::GUIObject* newParent)
    {        
        FUNC_DEBUG_ENTRY();
        
        #if USE_DEBUG
        DEBUG_LINE("Setting "<<this<<" parent from "<< CurrentObjectsReferences.GetObjectReference(Parent)<<" to "<<newParent);
        #endif

        ssGUI::GUIObject* originalParent = nullptr;

        //If setting parent to the same, just need to move this to the end of child
        if(newParent == CurrentObjectsReferences.GetObjectReference(Parent))
        {
            //DEBUG_LINE("Invalid parent detected");
            //DEBUG_EXIT_PROGRAM();

            newParent->FindChild(this);
            auto it = newParent->GetCurrentChildReferenceIterator();
            newParent->MoveChildrenIteratorToLast();
            auto lastIt = newParent->GetCurrentChildReferenceIterator();
            newParent->ChangeChildOrderToAfterPosition(it, lastIt);
            FUNC_DEBUG_EXIT();
            return;
        }

        //If invalid parent, flag it
        if(Parent != -1 && CurrentObjectsReferences.GetObjectReference(Parent) == nullptr)
            Parent = -1;
        //Remove child from the original parent
        else if(Parent != -1)
        {
            //Check if this object is the parent of the new parent
            if(newParent != nullptr)
            {
                ssGUI::GUIObject* checkParent = newParent;
                while (checkParent != nullptr)
                {
                    if(checkParent == static_cast<ssGUI::GUIObject*>(this))
                    {
                        DEBUG_LINE("Invalid parent detected");
                        DEBUG_EXIT_PROGRAM();
                        return;
                    }

                    checkParent = checkParent->GetParent();
                }
            }

            //Inform original parent to remove child
            CurrentObjectsReferences.GetObjectReference(Parent)->Internal_RemoveChild(this);
            
            //Record original parent
            originalParent = CurrentObjectsReferences.GetObjectReference(Parent);
        }

        //If the new parent is nullptr, then remove the reference of the original parent and flag it
        if (newParent == nullptr)
        {
            // if(Parent != -1)
            //     CurrentObjectsReferences.RemoveObjectReference(Parent);
            
            Parent = -1;
        }
        //Otherwise, add it to objects reference
        else
        {
            if(Parent != -1)
                CurrentObjectsReferences.RemoveObjectReference(Parent);
            
            Parent = CurrentObjectsReferences.AddObjectReference(newParent);
        }

        //Send event callback if any object is subscribed to on children removed
        while (originalParent != nullptr)
        {
            if(originalParent == newParent)
                break;

            if(originalParent->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME))
                originalParent->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME)->Notify(this);
            
            originalParent = originalParent->GetParent();
        }

        //Exit if this object is parented to nothing
        if(newParent == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Send event callback if any object is subscribed to on children add event
        ssGUI::GUIObject* currentParent = newParent;
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(this))
            {
                DEBUG_LINE("Invalid parent detected");
                DEBUG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME))
                currentParent->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME)->Notify(this);    
            
            currentParent = currentParent->GetParent();
        }

        //Inform new parent to add this as child
        newParent->Internal_AddChild(this);
        
        //Update local position
        SetGlobalPosition(GetGlobalPosition());
        
        //Send event callback if any object is subscribed to children added
        currentParent = CurrentObjectsReferences.GetObjectReference(Parent);
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(this))
            {
                DEBUG_LINE("Invalid parent detected");
                DEBUG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME))
                currentParent->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME)->Notify(this);    
            
            currentParent = currentParent->GetParent();
        }        

        FUNC_DEBUG_EXIT();
    }

    int BaseGUIObject::GetChildrenCount() const
    {
        return Children.size();
    }

    void BaseGUIObject::MoveChildrenIteratorToFirst()
    {
        CurrentChild = Children.begin();

        if(GetChildrenCount() == 0)
            CurrentChildIteratorEnd = true;
        else
            CurrentChildIteratorEnd = false;
    }

    void BaseGUIObject::MoveChildrenIteratorToLast()
    {
        CurrentChild = Children.end();
        
        if(GetChildrenCount() != 0)
        {
            CurrentChild--;
            CurrentChildIteratorEnd = false;
        }
        else
            CurrentChildIteratorEnd = true;
    }

    void BaseGUIObject::MoveChildrenIteratorNext()
    {
        if(CurrentChild == Children.end() || CurrentChild == --Children.end() || CurrentChildIteratorEnd)
        {
            CurrentChildIteratorEnd = true;
            return;
        }

        CurrentChild++;
        CurrentChildIteratorEnd = false;
    }

    void BaseGUIObject::MoveChildrenIteratorPrevious()
    {
        if(CurrentChild == Children.begin() || CurrentChildIteratorEnd)
        {
            CurrentChildIteratorEnd = true;
            return;
        }
        
        CurrentChild--;
        CurrentChildIteratorEnd = false;
    }

    bool BaseGUIObject::IsChildrenIteratorLast()
    {
        if(GetChildrenCount() == 0)
            return false;
        
        if(CurrentChild == --Children.end())
            return true;
        
        return false;
    }

    bool BaseGUIObject::IsChildrenIteratorFirst()
    {
        if(GetChildrenCount() == 0)
            return false;

        if(CurrentChild == Children.begin())
            return true;

        return false;
    }

    bool BaseGUIObject::IsChildrenIteratorEnd()
    {
        return CurrentChildIteratorEnd;
    }

    bool BaseGUIObject::FindChild(ssGUI::GUIObject* child)
    {
        if(child == nullptr)
            return false;
        
        std::list<ssGUIObjectIndex>::iterator it = Children.begin();
        std::list<ssGUIObjectIndex>::iterator endIt = Children.end();
        bool found = false;

        while (true)
        {
            if(it == endIt)
                break;
            
            if(CurrentObjectsReferences.GetObjectReference(*it) == child)
            {
                found = true;
                break;
            }

            it++;
        }

        CurrentChild = it;
        CurrentChildIteratorEnd = false;
        return found;
    }

    ssGUI::GUIObject* BaseGUIObject::GetCurrentChild()
    {
        if(!CurrentChildIteratorEnd)
        {
            if(CurrentObjectsReferences.GetObjectReference(*CurrentChild) == nullptr)
            {
                DEBUG_LINE("invalid child found");
                DEBUG_EXIT_PROGRAM();
                return nullptr;
            }
            
            return CurrentObjectsReferences.GetObjectReference(*CurrentChild);
        }
        else
            return nullptr;
    }

    std::list<ssGUIObjectIndex>::iterator BaseGUIObject::GetCurrentChildReferenceIterator()
    {
        return CurrentChild;
    }

    void BaseGUIObject::ChangeChildOrderToBeforePosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position)
    {
        Children.splice(position, Children, child);

        if(IsEventCallbackExist(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME)->Notify(CurrentObjectsReferences.GetObjectReference(*child));   
    }
    
    void BaseGUIObject::ChangeChildOrderToAfterPosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position)
    {
        Children.splice(++position, Children, child);

        if(IsEventCallbackExist(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME)->Notify(CurrentObjectsReferences.GetObjectReference(*child));   
    }   
    
    void BaseGUIObject::Internal_AddChild(ssGUI::GUIObject* obj)
    {
        if(FindChild(obj))
            return;

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.AddObjectReference(obj);
        
        Children.push_back(childIndex);
    }
    
    void BaseGUIObject::Internal_RemoveChild(ssGUI::GUIObject* obj)
    {
        FUNC_DEBUG_ENTRY();
        
        #if USE_DEBUG
        DEBUG_LINE(this<<" removing child "<<obj);
        #endif
        
        if(!FindChild(obj))
        {
            DEBUG_LINE("Remove failed");
            DEBUG_EXIT_PROGRAM();
            return;
        }
        
        std::list<ssGUIObjectIndex>::iterator it = GetCurrentChildReferenceIterator();

        if(CurrentChild == it)
        {
            if(Children.size() == 1 || CurrentChild == --Children.end() || CurrentChild == Children.begin())
            {
                CurrentChildIteratorEnd = true;
                CurrentChild = Children.end();
            }
            else
                CurrentChild--;
        }
        Children.remove(*it);
        #if USE_DEBUG
        DEBUG_LINE("Remove success");
        #endif
        FUNC_DEBUG_EXIT();
    }

    ssGUI::Enums::GUIObjectType BaseGUIObject::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::BASE_OBJECT;
    }

    ssGUI::Enums::AnchorType BaseGUIObject::GetAnchorType() const
    {
        return Anchor;
    }
    
    void BaseGUIObject::SetAnchorType(ssGUI::Enums::AnchorType anchorType)
    {
        Anchor = anchorType;
    }

    void BaseGUIObject::SetVisible(bool visible)
    {
        Visible = visible;
    }

    bool BaseGUIObject::IsVisible() const
    {
        return Visible;
    }

    void BaseGUIObject::SetUserCreated(bool created)
    {
        UserCreated = created;
    }

    bool BaseGUIObject::IsUserCreated() const
    {
        return UserCreated;
    }

    void BaseGUIObject::SetBackgroundColour(glm::u8vec4 color)
    {
        BackgroundColour = color;
    }

    glm::u8vec4 BaseGUIObject::GetBackgroundColour() const
    {
        //std::cout<<"GetBackgroundColour: "<<BackgroundColour.r<<","<<BackgroundColour.g<<","<<BackgroundColour.b<<","<<BackgroundColour.a<<"\n";
        
        return BackgroundColour;
    }

    void BaseGUIObject::Delete()
    {
        FUNC_DEBUG_ENTRY();

        #if USE_DEBUG
        DEBUG_LINE(this<<" object is getting deleted");
        #endif

        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        
        SetParent(nullptr);
        CurrentObjectsReferences.CleanUp();
        ObjectDelete = true;
        ssGUI::ssGUIManager::DeletedObjs.push_back(this);

        for(auto it : Extensions)
           delete it.second;
        
        for(auto it : EventCallbacks)
           delete it.second;

        FUNC_DEBUG_EXIT();
    }

    bool BaseGUIObject::Internal_IsDeleted() const
    {
        return ObjectDelete;
    }

    void BaseGUIObject::SetHeapAllocated(bool heap)
    {
        HeapAllocated = heap;
    }

    bool BaseGUIObject::IsHeapAllocated() const
    {
        return HeapAllocated;
    }

    std::vector<glm::ivec2>& BaseGUIObject::Extension_GetDrawingVerticies()
    {
        return DrawingVerticies;
    }

    std::vector<glm::ivec2>& BaseGUIObject::Extension_GetDrawingUVs()
    {
        return DrawingUVs;
    }

    std::vector<glm::u8vec4>& BaseGUIObject::Extension_GetDrawingColours()
    {
        return DrawingColours;
    }

    std::vector<int>& BaseGUIObject::Extension_GetDrawingCounts()
    {
        return DrawingCounts;
    }

    std::vector<ssGUI::DrawingProperty>& BaseGUIObject::Extension_GetDrawingProperties()
    {
        return DrawingProperties;
    }

    void BaseGUIObject::AddExtension(ssGUI::Extensions::Extension* extension)
    {
        if(IsExtensionExist(extension->GetExtensionName()))
            return;

        Extensions[extension->GetExtensionName()] = extension;
        extension->BindToObject(this);
    }

    ssGUI::Extensions::Extension* BaseGUIObject::GetExtension(std::string extensionName)
    {
        if(!IsExtensionExist(extensionName))
            return nullptr;

        return Extensions[extensionName];
    }

    bool BaseGUIObject::IsExtensionExist(std::string extensionName) const
    {
        return Extensions.find(extensionName) != Extensions.end();
    }

    void BaseGUIObject::RemoveExtension(std::string extensionName)
    {
        if(!IsExtensionExist(extensionName))
            return;

        ssGUI::Extensions::Extension* targetExtension = (*Extensions.find(extensionName)).second;
        // ssGUI::ObjectsReferences* ptr = targetExtension->Internal_GetObjectsReferences();
        // if(ptr != nullptr)
        //     ptr->CleanUp();
        
        Extensions.erase(extensionName);
        delete targetExtension;
    }

    void BaseGUIObject::AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback)
    {
        if(IsEventCallbackExist(eventCallback->GetEventCallbackName()))
            return;
        
        EventCallbacks[eventCallback->GetEventCallbackName()] = eventCallback;
    }

    ssGUI::EventCallbacks::EventCallback* BaseGUIObject::GetEventCallback(std::string eventCallbackName)
    {
        if(!IsEventCallbackExist(eventCallbackName))
            return nullptr;

        return EventCallbacks[eventCallbackName];
    }

    bool BaseGUIObject::IsEventCallbackExist(std::string eventCallbackName) const
    {
        return EventCallbacks.find(eventCallbackName) != EventCallbacks.end();
    }

    void BaseGUIObject::RemoveEventCallback(std::string eventCallbackName)
    {
        if(!IsEventCallbackExist(eventCallbackName))
            return;

        EventCallbacks.erase(eventCallbackName);
    }

    void BaseGUIObject::AddTag(std::string tag)
    {
        if(CurrentTags.find(tag) == CurrentTags.end())
            CurrentTags.insert(tag);
    }

    void BaseGUIObject::RemoveTag(std::string tag)
    {
        if(CurrentTags.find(tag) != CurrentTags.end())
            CurrentTags.erase(tag);
    }

    bool BaseGUIObject::HasTag(std::string tag) const
    {
        return CurrentTags.find(tag) != CurrentTags.end();
    }

    ObjectsReferences* BaseGUIObject::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    void BaseGUIObject::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();

        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        for(auto extension : Extensions)
            extension.second->Internal_Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);

        for(auto extension : Extensions)
            extension.second->Internal_Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();

        FUNC_DEBUG_EXIT();
    }

    void BaseGUIObject::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        for(auto extension : Extensions)
            extension.second->Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        endOfUpdate:;
        for(auto extension : Extensions)
            extension.second->Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        
        FUNC_DEBUG_EXIT();
    }

    GUIObject* BaseGUIObject::Internal_Clone(int currentindex, std::vector<ssGUI::GUIObject*>& objsToCopy, std::vector<ssGUI::GUIObject*>& copiedObjs, std::vector<int>& clonedParents, bool cloneChildren)
    {
        //Local copy
        BaseGUIObject* temp = new BaseGUIObject(*this);

        // if(clonedParents[currentindex] == -1)
        //     temp->SetParent(objsToCopy[0]->GetParent());
        // else 
        //     temp->SetParent(copiedObjs[clonedParents[currentindex]]);


        // if(cloneChildren)
        // {

        // }
    
        return temp;
    }

    GUIObject* BaseGUIObject::Clone(bool cloneChildren)
    {
        //TODO : implement clone children
        BaseGUIObject* temp = new BaseGUIObject(*this);
        


        // for(auto extension : Extensions)
        // {
        //     if(!temp->IsExtensionExist(extension.second->GetExtensionName()))
        //         temp->AddExtension(extension.second->Clone(temp));
        // }

        // for(auto eventCallback : EventCallbacks)
        // {
        //     std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();
            
        //     if(!temp->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
        //         temp->AddEventCallback(eventCallback.second->Clone(temp, originalObjs, tempVec));
        // }

        return temp;
    }
}
