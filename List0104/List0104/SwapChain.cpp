//=============================================================================
// SwapChain.cpp
// グラフィックス機能を提供するクラスが含まれます。
//=============================================================================
#include "Game.h"
#include <comdef.h>

using namespace Microsoft::WRL;

// このクラスのインスタンスを初期化します。
SwapChain::SwapChain(
	std::shared_ptr<Graphics> graphics, std::shared_ptr<MainWindow> window,
	DXGI_FORMAT swapChainFormat, DXGI_FORMAT depthStencilFormat) 
{
	// 関数の実行結果を受け取る変数
	HRESULT hr = S_OK;

	ComPtr<IDXGISwapChain1> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11ShaderResourceView> renderTargetResourceView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11ShaderResourceView> depthStencilResourceView;

	// 作成するスワップチェーンについての情報を格納
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = window->GetWidth();
	swapChainDesc.Height = window->GetHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT |
		DXGI_USAGE_SHADER_INPUT;	// シェーダーリソースとして使用することを設定
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	// 画面サイズを変える際(全画面表示とか)にスワップチェーンだけ変更する必要があるため
	//スワップチェーンを作成
	hr = graphics->GetDXGI_Factory()->CreateSwapChainForHwnd(graphics->GetDevice(), window->GetHandle(), &swapChainDesc, nullptr, nullptr, &swapChain);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// バックバッファーを取得
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
		hr = graphics->GetDevice()->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
		hr = graphics->GetDevice()->CreateShaderResourceView(backBuffer.Get(), NULL, &renderTargetResourceView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
	}

	// テクスチャとシェーダーリソースビューのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	DXGI_FORMAT resourceFormat = depthStencilFormat;
	//設定したフォーマットをテクスチャとリソース用に入れ替え
	switch (depthStencilFormat)
	{
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}
	// 深度ステンシルを作成
	{
		ComPtr<ID3D11Texture2D> depthStencil;
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = swapChainDesc.Width;
		depthStencilDesc.Height = swapChainDesc.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = textureFormat;
		depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // シェーダーリソースとして使用することを設定
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		hr = graphics->GetDevice()->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// 深度ステンシルにアクセスするためのビューを作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = depthStencilFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
		}
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		hr = graphics->GetDevice()->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &depthStencilView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// 深度ステンシルにシェーダーからアクセスするためのリソース ビューを作成
		D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
		depthStencilResourceViewDesc.Format = resourceFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
			depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
		}
		hr = graphics->GetDevice()->CreateShaderResourceView(depthStencil.Get(), &depthStencilResourceViewDesc, &depthStencilResourceView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		depthStencil.Reset();
	}

	// 初期化がすべて成功したらメンバー変数を更新する
	this->graphics = graphics;
	this->window = window;
	this->swapChain = swapChain;
	this->renderTargetView = renderTargetView;
	this->renderTargetResourceView = renderTargetResourceView;
	this->depthStencilView = depthStencilView;
	this->depthStencilResourceView = depthStencilResourceView;
}

// ID3D11RenderTargetView を取得します。
ID3D11RenderTargetView* SwapChain::GetRenderTargetView()
{
	return renderTargetView.Get();
}

// ID3D11DepthStencilView を取得します。
ID3D11DepthStencilView* SwapChain::GetDepthStencilView()
{
	return depthStencilView.Get();
}

// バックバッファーに描画したイメージをディスプレイに表示します。
void SwapChain::Present(UINT syncInterval)
{
	const auto hr = swapChain->Present(syncInterval, 0);
	if (FAILED(hr))
	{
		throw _com_error(hr);
	}
}
