//=============================================================================
// Graphics.cpp
// グラフィックス機能を提供するクラスが含まれます。
//=============================================================================
#include <GameLibrary/Game.h>
#include <GameLibrary/Utility.h>
#include <comdef.h>

using namespace GameLibrary;
using namespace Microsoft::WRL;

namespace {
#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	constexpr UINT factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	constexpr UINT creationFlags =
		D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG |
		D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUGGABLE;
	//constexpr UINT creationFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#else
	constexpr UINT factoryFlags = 0;
	constexpr UINT creationFlags = 0;
#endif

	constexpr D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
	};

	/// <summary>
	/// ハードウェアで実装されているディスプレイ サブシステムを取得します。
	/// </summary>
	/// <param name="factory">IDXGIFactory7 インターフェイス</param>
	/// <returns>ディスプレイ サブシステム</returns>
	ComPtr<IDXGIAdapter4> GetHardwareAdapter(IDXGIFactory7* factory)
	{
		ComPtr<IDXGIAdapter4> adapter;
		for (
			UINT adapterIndex = 0;
			SUCCEEDED(factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
			++adapterIndex)
		{
			auto desc = DXGI_ADAPTER_DESC3{};
			ThrowIfFailed(adapter->GetDesc3(&desc));
			// ソフトウェア アダプターは使用しない
			if (desc.Flags & DXGI_ADAPTER_FLAG3::DXGI_ADAPTER_FLAG3_SOFTWARE) {
				continue;
			}

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11DeviceContext> deviceContext;
			auto featureLevel = D3D_FEATURE_LEVEL{};
			// 作成できるか試してみるだけ
			if (SUCCEEDED(D3D11CreateDevice(
				adapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, NULL,
				creationFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)),
				D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext)))
			{
				break;
			}
		}
		return adapter;
	}
}

Graphics::Graphics() {
	InitGraphicsDevice();
}

void Graphics::InitGraphicsDevice() {
	HRESULT hr = S_OK;

	// ファクトリーを作成
	hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	dxgiAdapter = GetHardwareAdapter(dxgiFactory.Get());
	// WARP アダプター
	if (dxgiAdapter == nullptr) {
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter)));
	}

	auto adapterDesc = DXGI_ADAPTER_DESC3{};
	dxgiAdapter->GetDesc3(&adapterDesc);

	// ディスプレイ出力
	ComPtr<IDXGIOutput> currentOutput;
	for (
		UINT outputIndex = 0;
		SUCCEEDED(dxgiAdapter->EnumOutputs(outputIndex, &currentOutput));
		++outputIndex)
	{
		ComPtr<IDXGIOutput6> dxgiOutput;
		ThrowIfFailed(currentOutput.As(&dxgiOutput));
		ThrowIfFailed(dxgiOutput->GetDesc1(&outputDesc));
		switch (outputDesc.ColorSpace) {
		case DXGI_COLOR_SPACE_TYPE::DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709:
			break;
		case DXGI_COLOR_SPACE_TYPE::DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709:
			break;
		case DXGI_COLOR_SPACE_TYPE::DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020:
			break;
		case DXGI_COLOR_SPACE_TYPE::DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P2020:
			break;
		default:
			break;
		}
	}

	//// アダプターを列挙する
	//ComPtr<IDXGIAdapter4> currentAdapter;
	//for (
	//	UINT adapterIndex = 0;
	//	dxgiFactory->EnumAdapterByGpuPreference(
	//		adapterIndex,
	//		DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
	//		IID_PPV_ARGS(&currentAdapter)) != DXGI_ERROR_NOT_FOUND;
	//	adapterIndex++)
	//{
	//	DXGI_ADAPTER_DESC3 desc = {};
	//	currentAdapter->GetDesc3(&desc);
	//	// ソフトウェア アダプターは使用しない
	//	if (desc.Flags & DXGI_ADAPTER_FLAG3::DXGI_ADAPTER_FLAG3_SOFTWARE) {
	//		continue;
	//	}
	//	// 作成できるか試してみるだけ
	//	if (SUCCEEDED(D3D11CreateDevice(
	//		currentAdapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, NULL,
	//		creationFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)),
	//		D3D11_SDK_VERSION, NULL, NULL, NULL)))
	//	{
	//		break;
	//	}
	//}
	//dxgiAdapter = currentAdapter;
	//currentAdapter.Reset();

	// DXGIデバイスを継承
	ComPtr<IDXGIDevice4> dxgiDevice;

	// D3D11デバイスを継承
	ComPtr<ID3D11Device> currentDevice;

	ComPtr<ID3D11DeviceContext> currentDeviceContext;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL();

	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;

	// デバイス、デバイスコンテキストを作成
	hr = D3D11CreateDevice(
		dxgiAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL,
		creationFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)), D3D11_SDK_VERSION,
		&currentDevice, &featureLevel, &currentDeviceContext);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = currentDevice.As(&graphicsDevice);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = currentDeviceContext.As(&immediateContext);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	currentDevice.Reset();
	currentDeviceContext.Reset();

	//// デバイス、デバイスコンテキストを作成
	//hr = D3D11CreateDevice(
	//	NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
	//	&device, &featureLevel, &deviceContext);
	//if (FAILED(hr)) {
	//	throw _com_error(hr);
	//}

	//// Direct3D 11デバイスからDXGIインターフェイスを順に変換し取得（取得できない場合エラー）
	//// ID3D11Device -> IDXGIDevice1 -> IDXGIAdapter -> IDXGIAdapter1 -> IDXGIFactory1

	////ID3D11Device->IDXGIDevice1
	//if (FAILED(graphicsDevice.As(&dxgiDevice))) {
	//	throw _com_error(hr);
	//}
	//{
	//	//IDXGIDevice1->IDXGIAdapter
	//	ComPtr<IDXGIAdapter> adapter;
	//	if (FAILED(dxgiDevice->GetAdapter(&adapter))) {
	//		throw _com_error(hr);
	//	}
	//	//IDXGIAdapter->IDXGIAdapter1
	//	if (FAILED(adapter.As(&dxgiAdapter))) {
	//		throw _com_error(hr);
	//	}
	//}
	////IDXGIAdapter1->IDXGIFactory1
	//if (FAILED(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)))) {
	//	throw _com_error(hr);
	//}

	this->dxgiFactory = dxgiFactory;
	this->dxgiAdapter = dxgiAdapter;
	this->dxgiDevice = dxgiDevice;
	this->graphicsDevice = graphicsDevice;
	this->immediateContext = immediateContext;
	this->featureLevel = featureLevel;
}

// IDXGIFactory1 を取得します。
IDXGIFactory2* Graphics::GetDXGI_Factory()
{
	return dxgiFactory.Get();
}

// ID3D11Device を取得します。
ID3D11Device* Graphics::GetDevice()
{
	return graphicsDevice.Get();
}

// ID3D11DeviceContext を取得します。
ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return immediateContext.Get();
}
