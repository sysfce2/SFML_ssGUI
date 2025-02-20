#ifndef SSGUI_EXTENSION_MANAGER_H
#define SSGUI_EXTENSION_MANAGER_H

#include <unordered_map>
#include <type_traits>
#include <vector>
#include <string>
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/Extensions/Extension.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Renderer;
    class GUIObject;

    /*class: ssGUI::ExtensionManager
    This manages all the extensions for a GUI Object

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        std::unordered_map<std::string, ssGUI::Extensions::Extension*> Extensions;      //See <GetExtension>
        std::vector<std::string> ExtensionsDrawOrder;                                   //See <GetExtensionDrawOrder>
        std::vector<std::string> ExtensionsUpdateOrder;                                 //See <GetExtensionUpdateOrder>

        ssGUI::Renderer* CurrentRenderer;                                               //(Internal variable) Used to send redraw signal
        ssGUI::GUIObject* CurrentObject;                                                //(Internal variable) Used to register container
    =================================================================
    ============================== C++ ==============================
    ExtensionManager::ExtensionManager() :  Extensions(),
                                            ExtensionsDrawOrder(),
                                            ExtensionsUpdateOrder(),
                                            CurrentRenderer(nullptr),
                                            CurrentObject(nullptr)
    {}
    =================================================================
    */
    class ExtensionManager
    {
        protected:
            std::unordered_map<std::string, ssGUI::Extensions::Extension*> Extensions;      //See <GetExtension>
            std::vector<std::string> ExtensionsDrawOrder;                                   //See <GetExtensionDrawOrder>
            std::vector<std::string> ExtensionsUpdateOrder;                                 //See <GetExtensionUpdateOrder>

            ssGUI::Renderer* CurrentRenderer;                                               //(Internal variable) Used to send redraw signal
            ssGUI::GUIObject* CurrentObject;                                                //(Internal variable) Used to register container

            ExtensionManager(ExtensionManager const&);

        public:
            ExtensionManager();
            virtual ~ExtensionManager();

            virtual void SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::GUIObject* obj);
            
            //function: AddExtension
            //Adds an extension to this GUI Object. If the extension already exists, nothing will be modified.
            template<typename T>
            T* AddExtension()
            {
                if(std::is_base_of<ssGUI::Extensions::Extension, T>::value)
                {
                    if(IsAnyExtensionExist<T>())
                        return GetAnyExtension<T>();

                    auto* extension = ssGUI::Factory::Create<T>();
                    Extensions[extension->GetExtensionName()] = extension;
                    ExtensionsDrawOrder.push_back(extension->GetExtensionName());
                    ExtensionsUpdateOrder.push_back(extension->GetExtensionName());
                    extension->BindToObject(CurrentObject);
                    CurrentRenderer->RedrawObject();
                    return extension;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non extension type");
                    return nullptr;
                }                
            }
            
            //function: AddExtensionCopy
            //Adds an extension to this GUI Object by copying an already existing extension. 
            //If the extension already exists in this GUI object, nothing will be modified.
            template<typename T>
            T* AddExtensionCopy(T* copy)
            {
                if(std::is_base_of<ssGUI::Extensions::Extension, T>::value)
                {
                    if(IsExtensionExist(copy->GetExtensionName()))
                        return static_cast<T*>(GetExtension(copy->GetExtensionName()));

                    auto* extension = copy->Clone();
                    Extensions[extension->GetExtensionName()] = extension;
                    ExtensionsDrawOrder.push_back(extension->GetExtensionName());
                    ExtensionsUpdateOrder.push_back(extension->GetExtensionName());
                    extension->BindToObject(CurrentObject);
                    CurrentRenderer->RedrawObject();
                    return extension;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non extension type");
                    return nullptr;
                }                
            }
            
            //function: GetExtension
            //Gets the extension by the name of it. Nullptr will be returned if not found.
            virtual ssGUI::Extensions::Extension* GetExtension(std::string extensionName);

            //function: GetAnyExtension
            //Generic version of <GetExtension>. 
            //It has to be a different name as template function doesn't support inheritance.
            //If it had the same name, the derived version of GetExtension will "hide" the generic version of it
            //and will just throw an error saying the template function is not found.
            template <typename T>
            T* GetAnyExtension()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return static_cast<T*>(GetExtension(T::EXTENSION_NAME));
            }
            
            //function: GetListOfExtensions
            //Returns all the extensions on the GUI Object
            virtual std::vector<ssGUI::Extensions::Extension*> GetListOfExtensions();

            //function: IsExtensionExist
            //Returns true if the extension exists on this GUI Object
            virtual bool IsExtensionExist(std::string extensionName) const;

            //function: IsAnyExtensionExist
            //Generic version of <IsExtensionExist>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            bool IsAnyExtensionExist()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return IsExtensionExist(T::EXTENSION_NAME);
            }
            
            //function: RemoveExtension
            //Removes the extension by the name of it
            virtual void RemoveExtension(std::string extensionName);

            //function: RemoveAnyExtension
            //Generic version of <RemoveExtension>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            void RemoveAnyExtension()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                RemoveExtension(T::EXTENSION_NAME);
            }
            
            //function: GetExtensionsCount
            //Returns the number of extensions on this GUI Object
            virtual int GetExtensionsCount() const;
            
            //function: GetExtensionDrawOrder
            //Returns the draw order of the extension by the name of it
            virtual int GetExtensionDrawOrder(std::string extensionName) const;

            //function: GetAnyExtensionDrawOrder
            //Generic version of <GetExtensionDrawOrder>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            int GetAnyExtensionDrawOrder()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return GetExtensionDrawOrder(T::EXTENSION_NAME);
            }
            
            //function: ChangeExtensionDrawOrder
            //Changes the draw order of the extension by the name of it
            virtual void ChangeExtensionDrawOrder(std::string extensionName, int order);

            //function: ChangeAnyExtensionDrawOrder
            //Generic version of <ChangeExtensionDrawOrder>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            void ChangeAnyExtensionDrawOrder(int order)
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                ChangeExtensionDrawOrder(T::EXTENSION_NAME, order);
            }
            
            //function: GetExtensionUpdateOrder
            //Returns the update order of the extension by the name of it
            virtual int GetExtensionUpdateOrder(std::string extensionName) const;

            //function: GetAnyExtensionUpdateOrder
            //Generic version of <GetExtensionUpdateOrder>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            int GetAnyExtensionUpdateOrder()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return GetAnyExtensionUpdateOrder(T::EXTENSION_NAME);
            }
            
            //function: ChangeExtensionUpdateOrder
            //Changes the update order of the extension by the name of it
            virtual void ChangeExtensionUpdateOrder(std::string extensionName, int order);

            //function: ChangeAnyExtensionUpdateOrder
            //Generic version of <ChangeExtensionUpdateOrder>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            void ChangeAnyExtensionUpdateOrder(int order)
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                ChangeExtensionUpdateOrder(T::EXTENSION_NAME, order);
            }
    };
}

#endif