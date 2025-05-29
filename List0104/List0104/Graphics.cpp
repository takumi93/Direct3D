//=============================================================================
// Graphics.cpp
// グラフィックス機能を提供するクラスが含まれます。
//=============================================================================
#include "Game.h"
#include <comdef.h>

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
		//D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
	};
}

Graphics::Graphics() {
	HRESULT hr = S_OK;

	hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// アダプターを列挙する
	ComPtr<IDXGIAdapter4> currentAdapter;
	for (
		UINT adapterIndex = 0;
		dxgiFactory->EnumAdapterByGpuPreference(
			adapterIndex,
			DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&currentAdapter)) != DXGI_ERROR_NOT_FOUND;
		adapterIndex++)
	{
		DXGI_ADAPTER_DESC3 desc = {};
		currentAdapter->GetDesc3(&desc);
		// ソフトウェア アダプターは使用しない
		if (desc.Flags & DXGI_ADAPTER_FLAG3::DXGI_ADAPTER_FLAG3_SOFTWARE) {
			continue;
		}
		// 作成できるか試してみるだけ
		if (SUCCEEDED(D3D11CreateDevice(
			currentAdapter.Get(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, NULL,
			creationFlags, featureLevels, std::size(featureLevels),
			D3D11_SDK_VERSION, NULL, NULL, NULL)))
		{
			break;
		}
	}
	dxgiAdapter = currentAdapter;
	currentAdapter.Reset();

	// DXGIデバイスを継承
	ComPtr<IDXGIDevice1> dxgiDevice;

	// D3D11デバイスを継承
	ComPtr<ID3D11Device> device;

	ComPtr<ID3D11DeviceContext> deviceContext;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL();

	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;

	// デバイス、デバイスコンテキストを作成
	hr = D3D11CreateDevice(
		dxgiAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL,
		creationFlags, featureLevels, std::size(featureLevels), D3D11_SDK_VERSION,
		&device, &featureLevel, &deviceContext);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = device.As(&graphicsDevice);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = deviceContext.As(&immediateContext);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

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
