#include "framework.h"
#include "ResourceManager.h"
#include "FactoryManager.h"

#include <wincodec.h>

wchar_t ResourceManager::resourcePath[255] = {};
ID2D1HwndRenderTarget* ResourceManager::renderTarget = nullptr;
std::unordered_map<std::wstring, std::shared_ptr<IResource>> ResourceManager::resourceStorage = {};

void ResourceManager::Initialize(ID2D1HwndRenderTarget* _renderTarget)
{
	renderTarget = _renderTarget;
	InitialPath();
}

ID2D1Bitmap* ResourceManager::CreateID2D1Bitmap(std::wstring _key)
{
    HRESULT hr;
    ID2D1Bitmap* D2DBitmap = nullptr;
    IWICBitmapDecoder* pDecoder = NULL;
    IWICFormatConverter* pConverter = NULL;

    hr = FactoryManager::GetImagingFactory().CreateDecoderFromFilename(
        _key.c_str(),                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = NULL;
    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }

    //Step 3: Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
        hr = FactoryManager::GetImagingFactory().CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = renderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &D2DBitmap);
    }


    // 파일을 사용할때마다 다시 만든다.
    if (pConverter)
        pConverter->Release();

    if (pDecoder)
        pDecoder->Release();

    if (pFrame)
        pFrame->Release();

    return D2DBitmap;
}

void ResourceManager::InitialPath()
{
	wcscat_s(resourcePath, 255, L"../");
}
