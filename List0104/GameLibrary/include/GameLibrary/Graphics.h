#pragma once

#include <Windows.h>
#include <iterator>
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <memory>

namespace GameLibrary
{
	struct Vertex_Sprite
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 TexCoord;

		static constexpr UINT GetSize() { return static_cast<UINT>(sizeof(Vertex_Sprite)); }

		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0,    DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static constexpr UINT NumElements = static_cast<UINT>(std::size(InputElementDescs));
	};

	// グラフィックス機能を表します。
	class Graphics final
	{
	public:
		// このクラスのインスタンスを初期化します。
		Graphics(bool forceVSync, bool useWarpAdapter);
		~Graphics() = default;

		// IDXGIFactory1 を取得します。
		IDXGIFactory2* GetDXGI_Factory();
		// ID3D11Device を取得します。
		ID3D11Device* GetDevice();
		// ID3D11DeviceContext を取得します。
		ID3D11DeviceContext* GetDeviceContext();

		/// <summary>
		/// ティアリングする時に使用する判定を取得します。
		/// </summary>
		/// <returns></returns>
		bool IsTearingSupported() const noexcept { return allowTearing; }

	private:
		// グラフィックデバイスを作成します。
		void InitGraphicsDevice(bool forceVSync, bool useWarpAdapter);

		// ファクトリーを継承
		// DXGI 1.6のファクトリー
		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
		// アダプターを継承
		// DXGI 1.1のアダプター
		Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter;
		// DXGI 1.1のデバイス
		Microsoft::WRL::ComPtr<IDXGIDevice4> dxgiDevice;
		// Direct3D 11のデバイス
		Microsoft::WRL::ComPtr<ID3D11Device5> graphicsDevice;
		// Direct3D 11のデバイス コンテキスト
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> deviceContext;
		// Direct3D 11の機能レベル
		D3D_FEATURE_LEVEL featureLevel = {};

		bool allowTearing = false;

		DXGI_OUTPUT_DESC1 outputDesc = {};
	};

	// スワップ チェーンを管理してレンダーターゲットの機能を提供するクラスを表します。
	class SwapChain
	{
	public:
		// このクラスのインスタンスを初期化します。
		SwapChain(std::shared_ptr<Graphics> graphics, HWND hWnd, int width, int height);
		~SwapChain() = default;

		// ID3D11RenderTargetView を取得します。
		ID3D11RenderTargetView* GetRenderTargetView();
		// ID3D11DepthStencilView を取得します。
		ID3D11DepthStencilView* GetDepthStencilView();

		UINT GetWidth() const noexcept;
		UINT GetHeight() const noexcept;

		void BeginRender(ID3D11DeviceContext4* deviceContext, DirectX::FXMVECTOR clearColor);

		// バックバッファーに描画したイメージをディスプレイに表示します。
		void Present(UINT syncInterval);

	private:
		// 継承
		std::shared_ptr<Graphics> graphics;

		HWND window = nullptr;

		UINT width = 0;
		UINT height = 0;

		// スワップチェーン
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

		// レンダーターゲット
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView1> renderTargetView;
		// 深度ステンシル
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		// 深度バッファーをシェーダーで利用するためのリソース ビュー
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> depthShaderResourceView;
		// ステンシル バッファーをシェーダーで利用するためのリソース ビュー
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> stencilShaderResourceView;
	};

	class GraphicsResource
	{
	public:
		// noexceptで例外が出ない事を指している（作った人以外が分かりやすいようにnoexceptを入れている）
		GraphicsResource(ID3D11Device5* graphicsDevice) noexcept;
		virtual ~GraphicsResource() = default;

		ID3D11Device5* GetDevice() noexcept;

	private:
		// グラフィックデバイス
		Microsoft::WRL::ComPtr<ID3D11Device5> graphicsDevice;
	};
}