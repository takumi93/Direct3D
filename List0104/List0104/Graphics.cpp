//=============================================================================
// Graphics.cpp
// グラフィックス機能を提供するクラスが含まれます。
//=============================================================================
#include "Game.h"
#include <comdef.h>

using namespace Microsoft::WRL;

Graphics::Graphics() {
	HRESULT hr = S_OK;
	ComPtr<IDXGIFactory1> dxgiFactory;
	ComPtr<IDXGIAdapter1> dxgiAdapter;
	ComPtr<IDXGIDevice1> dxgiDevice;
	ComPtr<ID3D11Device> graphicsDevice;
	ComPtr<ID3D11DeviceContext> immediateContext;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL();

	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;

#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// デバイス、デバイスコンテキストを作成
	hr = D3D11CreateDevice(
		NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
		&graphicsDevice, &featureLevel, &immediateContext);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// Direct3D 11デバイスからDXGIインターフェイスを順に変換し取得（取得できない場合エラー）
	// ID3D11Device -> IDXGIDevice1 -> IDXGIAdapter -> IDXGIAdapter1 -> IDXGIFactory1

	//ID3D11Device->IDXGIDevice1
	if (FAILED(graphicsDevice.As(&dxgiDevice))) {
		throw _com_error(hr);
	}
	{
		//IDXGIDevice1->IDXGIAdapter
		ComPtr<IDXGIAdapter> adapter;
		if (FAILED(dxgiDevice->GetAdapter(&adapter))) {
			throw _com_error(hr);
		}
		//IDXGIAdapter->IDXGIAdapter1
		if (FAILED(adapter.As(&dxgiAdapter))) {
			throw _com_error(hr);
		}
	}
	//IDXGIAdapter1->IDXGIFactory1
	if (FAILED(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)))) {
		throw _com_error(hr);
	}

	this->dxgiFactory = dxgiFactory;
	this->dxgiAdapter = dxgiAdapter;
	this->dxgiDevice = dxgiDevice;
	this->graphicsDevice = graphicsDevice;
	this->immediateContext = immediateContext;
	this->featureLevel = featureLevel;
}

// IDXGIFactory1 を取得します。
IDXGIFactory1* Graphics::GetDXGI_Factory()
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
