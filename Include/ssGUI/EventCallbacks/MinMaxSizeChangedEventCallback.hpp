#ifndef SSGUI_MIN_MAX_SIZE_CHANGED_EVENT_CALLBACK_H
#define SSGUI_MIN_MAX_SIZE_CHANGED_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback
    //This event callback is triggered *after* min or max size of a GUI object has changed. Container will be the source for triggering this event callback.
    class MinMaxSizeChangedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            MinMaxSizeChangedEventCallback() = default;
            MinMaxSizeChangedEventCallback(MinMaxSizeChangedEventCallback const &) = default;
            MinMaxSizeChangedEventCallback& operator=(MinMaxSizeChangedEventCallback const &) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};
        
        public:
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual MinMaxSizeChangedEventCallback* Clone(bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif