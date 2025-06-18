//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <Windows.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <string>
#include <iterator>
#include <memory>

using Microsoft::WRL::ComPtr;

// アプリケーションのエントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	HRESULT hr = S_OK;

	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		return 0;
	}

	{
		// ファクトリーの作成
		ComPtr<IWICImagingFactory2> factory;
		hr = CoCreateInstance(
			CLSID_WICImagingFactory2,
			NULL, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&factory));
		if (FAILED(hr)) {
			return 0;
		}

		// デコードの作成
		ComPtr<IWICBitmapDecoder> decorder;
		hr = factory->CreateDecoderFromFilename(
			L"Sample.png",
			NULL, 
			GENERIC_READ, 
			WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
			&decorder);
		if (FAILED(hr)) {
			return 0;
		}

		// BitMapフレームを取得
		//UINT frameCount = 0;
		//hr = decorder->GetFrameCount(&frameCount);
		//if (FAILED(hr)) {
		//	return 0;
		//}
		ComPtr<IWICBitmapFrameDecode> frame;
		hr = decorder->GetFrame(0, &frame);
		if (FAILED(hr)) {
			return 0;
		}

		// FormatConverter
		ComPtr<IWICFormatConverter> converter;
		hr = factory->CreateFormatConverter(&converter);
		if (FAILED(hr)) {
			return 0;
		}
		hr = converter->Initialize(
			frame.Get(),
			GUID_WICPixelFormat24bppBGR,
			WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) {
			return 0;
		}

		UINT width = 0;
		UINT height = 0;
		hr = converter->GetSize(&width, &height);
		if (FAILED(hr)) {
			return 0;
		}

		// 画像フォーマットを取得
		WICPixelFormatGUID wicFormat = {};
		hr = converter->GetPixelFormat(&wicFormat);
		if (FAILED(hr)) {
			return 0;
		}

		struct WIC2DXGI_Format
		{
			const GUID& wic;
			DXGI_FORMAT dxgi;
		};

		// 画像フォーマットからdxgiフォーマットに変換する一覧を作成
		WIC2DXGI_Format wic2dxgi_formats[] = {
			{GUID_WICPixelFormat128bppRGBAFloat, DXGI_FORMAT_R32G32B32A32_FLOAT},
			{GUID_WICPixelFormat64bppRGBAHalf, DXGI_FORMAT_R16G16B16A16_FLOAT},
			{GUID_WICPixelFormat64bppRGBA, DXGI_FORMAT_R16G16B16A16_UNORM},
			{GUID_WICPixelFormat32bppRGBA, DXGI_FORMAT_R8G8B8A8_UNORM},
			{GUID_WICPixelFormat32bppBGRA, DXGI_FORMAT_B8G8R8A8_UNORM},
			{GUID_WICPixelFormat32bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM},
			{GUID_WICPixelFormat24bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM},
			{GUID_WICPixelFormat32bppRGBA1010102XR, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM},
			{GUID_WICPixelFormat32bppRGBA1010102, DXGI_FORMAT_R10G10B10A2_UNORM},
			{GUID_WICPixelFormat32bppRGBE, DXGI_FORMAT_R9G9B9E5_SHAREDEXP},
			{GUID_WICPixelFormat16bppBGRA5551, DXGI_FORMAT_B5G5R5A1_UNORM},
			{GUID_WICPixelFormat16bppBGR565, DXGI_FORMAT_B5G6R5_UNORM},
			{GUID_WICPixelFormat32bppGrayFloat, DXGI_FORMAT_R32_FLOAT},
			{GUID_WICPixelFormat16bppGrayHalf, DXGI_FORMAT_R16_FLOAT},
			{GUID_WICPixelFormat16bppGray, DXGI_FORMAT_R16_UNORM},
			{GUID_WICPixelFormat8bppGray, DXGI_FORMAT_R8_UNORM},
			{GUID_WICPixelFormat8bppAlpha, DXGI_FORMAT_A8_UNORM},
			{GUID_WICPixelFormat96bppRGBFloat, DXGI_FORMAT_R32G32B32_FLOAT},
		};

		// dxgiフォーマットを宣言
		auto dxgiFormat = DXGI_FORMAT{};

		// dxgiフォーマット一覧から取得した画像フォーマットと同じものを探す
		for (size_t index = 0; index < std::size(wic2dxgi_formats); index++)
		{
			auto& wic2dxgi_format = wic2dxgi_formats[index];
			if (InlineIsEqualGUID(wicFormat, wic2dxgi_format.wic)) {
				dxgiFormat = wic2dxgi_format.dxgi;
			}
			// ない場合はUNKNOWNとする
			//dxgiFormat = DXGI_FORMAT_UNKNOWN;
		}

		const auto bufferSize = width * height * 4;
		std::unique_ptr<BYTE[]> pixelData = std::make_unique<BYTE[]>(bufferSize);
		converter->CopyPixels(nullptr, width * 4, width* height * 4, pixelData.get());

		pixelData.reset();
	}

	// 解放処理
	CoUninitialize();

	return 0;
}