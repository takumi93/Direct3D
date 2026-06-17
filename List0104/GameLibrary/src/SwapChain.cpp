//=============================================================================
// SwapChain.cpp
// グラフィックス機能を提供するクラスが含まれます。
//=============================================================================
//#include <GameLibrary/Game.h>
#include <GameLibrary/Graphics.h>
#include <GameLibrary/Utility.h>
#include <comdef.h>

using namespace GameLibrary;
using namespace Microsoft::WRL;

// このクラスのインスタンスを初期化します。
SwapChain::SwapChain(
	std::shared_ptr<Graphics> graphics, HWND window, int width, int height) 
	: graphics(graphics)
	, window(window)
	, width(static_cast<UINT>(width))
	, height(static_cast<UINT>(height))
{
	ComPtr<IDXGISwapChain1> currentSwapChain;
	ComPtr<ID3D11Device5> device;
	graphics->GetDevice()->QueryInterface(IID_PPV_ARGS(&device));

	// 作成するスワップチェーンについての情報を格納
	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC1{
		.Width = static_cast<UINT>(width),
		.Height = static_cast<UINT>(height),

		//.Format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT,
		//.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
		.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		//.Format = DXGI_FORMAT::DXGI_FORMAT_R10G10B10A2_UNORM,

		.Stereo = FALSE,
		.SampleDesc = {.Count = 1, .Quality = 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,

		//.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH,
		.Scaling = DXGI_SCALING::DXGI_SCALING_NONE,
		//.Scaling = DXGI_SCALING::DXGI_SCALING_ASPECT_RATIO_STRETCH,

		.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = 0,
	};

	if (graphics->IsTearingSupported()) {
		swapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}

	// フルスクリーン設定
	const auto fullscreenDesc = DXGI_SWAP_CHAIN_FULLSCREEN_DESC{
		.RefreshRate = {.Numerator = 0, .Denominator = 0, },
		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED,
		.Windowed = TRUE,
	};

	// 画面サイズを変える際(全画面表示とか)にスワップチェーンだけ変更する必要があるため
	//スワップチェーンを作成
	{
		ThrowIfFailed(graphics->GetDXGI_Factory()->CreateSwapChainForHwnd(
			graphics->GetDevice(), window, 
			&swapChainDesc, &fullscreenDesc, nullptr,
			&currentSwapChain));

		ThrowIfFailed(currentSwapChain.As(&swapChain));
	}

	// バックバッファーを取得
	{
		ComPtr<ID3D11Texture2D1> backBuffer;
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

		auto backBufferDesc = D3D11_TEXTURE2D_DESC1{};
		backBuffer->GetDesc1(&backBufferDesc);

		// レンダー ターゲット
		auto format = DXGI_FORMAT{};
		switch (backBufferDesc.Format) {
		case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
			format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
			format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT::DXGI_FORMAT_R10G10B10A2_UNORM:
		default:
			format = backBufferDesc.Format;
			break;
		}

		const auto renderTargetViewDesc = D3D11_RENDER_TARGET_VIEW_DESC1{
			.Format = format,
			.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D,
			.Texture2D = {.MipSlice = 0, .PlaneSlice = 0, },
		};

		// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
		ThrowIfFailed(device->CreateRenderTargetView1(backBuffer.Get(), &renderTargetViewDesc, &renderTargetView));
		backBuffer.Reset();
	}

	//// テクスチャとシェーダーリソースビューのフォーマットを設定
	//DXGI_FORMAT textureFormat = depthStencilFormat;
	//DXGI_FORMAT resourceFormat = depthStencilFormat;
	////設定したフォーマットをテクスチャとリソース用に入れ替え
	//switch (depthStencilFormat)
	//{
	//case DXGI_FORMAT_D16_UNORM:
	//	textureFormat = DXGI_FORMAT_R16_TYPELESS;
	//	resourceFormat = DXGI_FORMAT_R16_UNORM;
	//	break;
	//case DXGI_FORMAT_D24_UNORM_S8_UINT:
	//	textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
	//	resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	//	break;
	//case DXGI_FORMAT_D32_FLOAT:
	//	textureFormat = DXGI_FORMAT_R32_TYPELESS;
	//	resourceFormat = DXGI_FORMAT_R32_FLOAT;
	//	break;
	//case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	//	textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
	//	resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	//	break;
	//}

	// 深度ステンシルを作成
	{
		ComPtr<ID3D11Texture2D1> depthStencilBuffer;
		const auto depthStencilDesc = D3D11_TEXTURE2D_DESC1{
			.Width = swapChainDesc.Width,
			.Height = swapChainDesc.Height,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS,
			.SampleDesc = swapChainDesc.SampleDesc,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, // シェーダーリソースとして使用することを設定
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED,
		};
		ThrowIfFailed(device->CreateTexture2D1(&depthStencilDesc, NULL, &depthStencilBuffer));

		// 深度ステンシルにアクセスするためのビューを作成
		const auto depthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC{
			.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
			.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D,
			.Flags = 0,
			.Texture2D = {
				.MipSlice = 0, },
			};
		ThrowIfFailed(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, &depthStencilView));

		// 深度ステンシルにシェーダーからアクセスするためのリソース ビューを作成
		const auto depthSRV_Desc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
			.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
			.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
			.Texture2D = {
				.MostDetailedMip = 0,
				.MipLevels = 1,
				.PlaneSlice = 0, }
		};
		ThrowIfFailed(device->CreateShaderResourceView1(depthStencilBuffer.Get(), &depthSRV_Desc, &depthShaderResourceView));
		
		const auto stencilSRV_Desc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
			.Format = DXGI_FORMAT::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
			.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
			.Texture2D = {
				.MostDetailedMip = 0,
				.MipLevels = 1,
				.PlaneSlice = 0, }
		};
		ThrowIfFailed(device->CreateShaderResourceView1(depthStencilBuffer.Get(), &stencilSRV_Desc, &stencilShaderResourceView));

		depthStencilBuffer.Reset();
	}
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

void SwapChain::BeginRender(ID3D11DeviceContext4* deviceContext, DirectX::FXMVECTOR clearColor) 
{
	// レンダーターゲットを設定
	ID3D11RenderTargetView* renderTargetViews[] = {
		renderTargetView.Get(),
	};

	deviceContext->OMSetRenderTargets(
		static_cast<UINT>(std::size(renderTargetViews)),
		renderTargetViews,
		depthStencilView.Get()
	);

	// 画面をクリアー
	deviceContext->ClearRenderTargetView(
		renderTargetView.Get(),
		DirectX::XMColorSRGBToRGB(clearColor).m128_f32
	);

	deviceContext->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	D3D11_VIEWPORT viewport{
	.TopLeftX = 0,
	.TopLeftY = 0,
	.Width = static_cast<float>(width),
	.Height = static_cast<float>(height),
	.MinDepth = 0.0f,
	.MaxDepth = 1.0f,
	};

	deviceContext->RSSetViewports(1, &viewport);
}

/// <summary>
/// バックバッファーに描画したイメージをディスプレイに表示します。
/// </summary>
/// <param name="syncInterval"></param>
void SwapChain::Present(UINT syncInterval)
{
	UINT presentFlags = 0;

	if (graphics->IsTearingSupported()){
		presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
	}

	const auto presentParameters = DXGI_PRESENT_PARAMETERS{
		.DirtyRectsCount = 0,
		.pDirtyRects = nullptr,
		.pScrollRect = nullptr,
		.pScrollOffset = nullptr,
	};

	ThrowIfFailed(swapChain->Present1(syncInterval, presentFlags, &presentParameters));
}
