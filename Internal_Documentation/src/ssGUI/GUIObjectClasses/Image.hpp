#ifndef SSGUI_IMAGE
#define SSGUI_IMAGE

#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Enums/ImageFitting.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "SFML/Graphics.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Image
    A class that can show the content of <ImageData>

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::ImageData* ImageData;
        ssGUI::Enums::ImageFitting Fitting;
    =================================================================
    ============================== C++ ==============================
    Image::Image() : ImageData(nullptr), Fitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_AREA) 
    {
        //AddExtension(new ssGUI::Extensions::Border());
    }
    =================================================================
    */
    class Image : public Widget
    {
        private:
            Image& operator=(Image const& other);
        
        protected:
            ssGUI::ImageData* ImageData;
            ssGUI::Enums::ImageFitting Fitting;
            Image(Image const& other);

            virtual void ConstructRenderInfo() override;
            
        public:
            Image();
            virtual ~Image() override;

            //function: GetImageData
            //Gets the ImageData object used by this Image widget. Returns nullptr if no ImageData is used
            virtual ssGUI::ImageData* GetImageData() const;

            //function: SetImageData
            //Sets the ImageData object to be used by this Image widget
            virtual void SetImageData(ssGUI::ImageData* imageData);

            //function: GetFitting
            //Returns how the image will be fitted inside this Image Widget
            virtual ssGUI::Enums::ImageFitting GetFitting() const;

            //function: SetFitting
            //Sets how the image will be fitted inside this Image Widget
            virtual void SetFitting(ssGUI::Enums::ImageFitting fitting);

            //Overriding widget
            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Image* Clone(bool cloneChildren) override;

    };
}


#endif