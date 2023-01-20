#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/TestBase.hpp"
#include <thread>

ssGUI::Backend::BackendImageInterface* TestImage = nullptr;

SSGUI_TEST_INIT();

void SetUp()
{
    TestImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(TestImage);
}

void IsValidTest()
{
    SSGUI_TEST_OUTPUT_ASSERT(__func__, !TestImage->IsValid());
}

void LoadFromPathTest()
{
    //TODO
    SSGUI_TEST_OUTPUT_SKIP(__func__);
}

void LoadImgFileFromMemoryTest()
{
    //TODO
    SSGUI_TEST_OUTPUT_SKIP(__func__);
}

const int imgWidth = 64;
const int imgHeight = 64;
const int columnWidth = imgWidth / 4;

void LoadRawFromMemoryTest()
{
    uint8_t dummyImg[imgWidth * imgHeight * 4] = {0};
    
    
    for(int y = 0; y < imgHeight; y++)
    {
        for(int x = 0; x < imgWidth; x++)
        {
            dummyImg[(y * imgWidth + x) * 4 + 0] = x < columnWidth ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 1] = x >= columnWidth && x < columnWidth * 2 ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 2] = x >= columnWidth * 2 && x < columnWidth * 3 ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 3] = x >= columnWidth * 3 ? 127 : 255;
        }
    }
    
    TestImage->LoadRawFromMemory(dummyImg, ssGUI::ImageFormat(), glm::ivec2(64, 64));
    
    SSGUI_TEST_OUTPUT_ASSERT(__func__, TestImage->IsValid());
}

void GetSizeTest()
{
    SSGUI_TEST_OUTPUT_ASSERT(   __func__, 
                                TestImage->GetSize().x == imgWidth && TestImage->GetSize().y == imgHeight);
}

void GetPixelPtrTest()
{
    ssGUI::ImageFormat format;
    uint8_t* pixelPtr = static_cast<uint8_t*>(TestImage->GetPixelPtr(format));
    
    SSGUI_TEST_OUTPUT_ASSERT(   __func__ + std::string("(Format Valid)"),
                                format.BitDepthPerChannel == 8 &&
                                format.ImgType == ssGUI::Enums::ImageType::RGB &&
                                format.HasAlpha == true &&
                                format.IndexR == 0 &&
                                format.IndexG == 1 &&
                                format.IndexB == 2 &&
                                format.IndexA == 3);

    SSGUI_TEST_OUTPUT_ASSERT(   __func__ + std::string("(Pixel Valid)"),
                                pixelPtr != nullptr &&
                                //Red
                                pixelPtr[0 * 4 + 0] == 255 &&
                                pixelPtr[0 * 4 + 1] == 0 &&
                                pixelPtr[0 * 4 + 2] == 0 &&
                                pixelPtr[0 * 4 + 3] == 255 &&
                                
                                //Green
                                pixelPtr[columnWidth * 4 + 0] == 0 &&
                                pixelPtr[columnWidth * 4 + 1] == 255 &&
                                pixelPtr[columnWidth * 4 + 2] == 0 &&
                                pixelPtr[columnWidth * 4 + 3] == 255 &&
                                
                                //Blue
                                pixelPtr[columnWidth*2 * 4 + 0] == 0 &&
                                pixelPtr[columnWidth*2 * 4 + 1] == 0 &&
                                pixelPtr[columnWidth*2 * 4 + 2] == 255 &&
                                pixelPtr[columnWidth*2 * 4 + 3] == 255 &&
                                
                                //Green
                                pixelPtr[columnWidth*3 * 4 + 0] == 0 &&
                                pixelPtr[columnWidth*3 * 4 + 1] == 0 &&
                                pixelPtr[columnWidth*3 * 4 + 2] == 0 &&
                                pixelPtr[columnWidth*3 * 4 + 3] == 127);
}

void CloneTest()
{
    ssGUI::Backend::BackendImageInterface* clonedImg = TestImage->Clone();
    
    SSGUI_TEST_OUTPUT_ASSERT(   __func__,
                                clonedImg != nullptr &&
                                clonedImg->GetSize().x == TestImage->GetSize().x &&
                                clonedImg->GetSize().y == TestImage->GetSize().y);

    ssGUI::Factory::Dispose(clonedImg);
}

int main()
{
    SetUp();
    
    SSGUI_TEST_TITLE(SSGUI_TEST_FILE_NAME());
    
    try
    {
        //Can't test GetRawHandle
        IsValidTest();
        LoadFromPathTest();
        LoadImgFileFromMemoryTest();
        LoadRawFromMemoryTest();
        GetSizeTest();
        GetPixelPtrTest();
        //Can't test AddBackendDrawingLinking
        //Can't test RemoveBackendDrawingLinking
        CloneTest();
    }
    catch(...)
    {
        CleanUp();
        throw;
    }
    
    CleanUp();
    
    SSGUI_TEST_END();
}

