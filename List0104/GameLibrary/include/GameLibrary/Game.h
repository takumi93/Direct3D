//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>

#include <memory>
#include <string>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace GameLibrary {

	//=============================================================================
	// 構造体
	// 
	// 
	//=============================================================================

	// 作成するウィンドウの情報
	struct ProjectSettings
	{
		std::wstring Version = L"1.0.0";
		std::wstring Title = L"Game Title";
		int Width = 640;
		int Height = 480;
		bool ForceVSync = false;
		bool UseWarpAdapter = false;
	};

	// 位置座標のみを頂点情報に持つデータを表します。
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// 位置座標

		// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
		static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 位置座標とカラーを頂点情報に持つデータを表します。
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT4 color;	// 頂点カラー

		// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
		static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 位置座標と法線ベクトルを頂点情報に持つデータを表します。
	struct VertexPositionNormal
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル

		// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
		static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	// 位置座標と法線ベクトルとテクスチャー座標を頂点情報に持つデータを表します。
	struct VertexPositionNormalTexture
	{
		DirectX::XMFLOAT3 position;				// 位置座標
		DirectX::XMFLOAT3 normal;				// 法線ベクトル
		DirectX::XMFLOAT2 textureCoordinate;	// テクスチャー座標

		// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
		static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	//=============================================================================
	// クラス
	// 
	// 
	//=============================================================================

	// グラフィックス機能を表します。
	class Graphics final
	{
	public:
		// このクラスのインスタンスを初期化します。
		Graphics();
		~Graphics() = default;

		// IDXGIFactory1 を取得します。
		IDXGIFactory2* GetDXGI_Factory();
		// ID3D11Device を取得します。
		ID3D11Device* GetDevice();
		// ID3D11DeviceContext を取得します。
		ID3D11DeviceContext* GetDeviceContext();

	private:
		// グラフィックデバイスを作成します。
		void InitGraphicsDevice();

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
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> immediateContext;
		// Direct3D 11の機能レベル
		D3D_FEATURE_LEVEL featureLevel = {};

	protected:
		DXGI_OUTPUT_DESC1 outputDesc = {};
	};

	// 頂点シェーダーを表します。
	class BasicVertexShader
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		BasicVertexShader(std::shared_ptr<Graphics> graphics);
		~BasicVertexShader() = default;

		// このシェーダーのバイトコードを取得します。
		const BYTE* GetBytecode();
		// バイトコードのサイズを取得します。
		size_t GetBytecodeLength();

		// D3D11のネイティブポインターを取得します。
		ID3D11VertexShader* GetNativePointer();

	private:
		std::shared_ptr<Graphics> graphics;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	};

	// ジオメトリーシェーダーを表します。
	class BasicGeometryShader
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		BasicGeometryShader(std::shared_ptr<Graphics> graphics);
		~BasicGeometryShader() = default;

		// D3D11のネイティブポインターを取得します。
		ID3D11GeometryShader* GetNativePointer();

	private:
		std::shared_ptr<Graphics> graphics;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
	};

	// ピクセルシェーダーを表します。
	class BasicPixelShader
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		BasicPixelShader(std::shared_ptr<Graphics> graphics);
		~BasicPixelShader() = default;

		// D3D11のネイティブポインターを取得します。
		ID3D11PixelShader* GetNativePointer();

	private:
		std::shared_ptr<Graphics> graphics;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	};

	// 頂点バッファーを表します。
	class VertexBuffer
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		VertexBuffer(std::shared_ptr<Graphics> graphics, UINT byteWidth);

		~VertexBuffer() = default;

		// バッファーにデータを設定します。
		void SetData(const void* data);

		// D3D11のネイティブポインターを取得します。
		ID3D11Buffer* GetNativePointer();

	private:
		std::shared_ptr<Graphics> graphics;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		UINT stride = 0;
	};

	// インデックスバッファーを表します。
	class IndexBuffer
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		IndexBuffer(std::shared_ptr<Graphics> graphics, UINT byteWidth);
		~IndexBuffer() = default;

		// バッファーにデータを設定します。
		void SetData(const void* data);

		// D3D11のネイティブポインターを取得します。
		ID3D11Buffer* GetNativePointer();

	private:
		std::shared_ptr<Graphics> graphics;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	// 定数バッファーを表します。
	class ConstantBuffer
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		ConstantBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth);
		~ConstantBuffer() = default;

		// バッファーにデータを設定します。
		void SetData(const void* data);

		// D3D11のネイティブポインターを取得します。
		ID3D11Buffer* GetNativePointer();

	private:
		std::shared_ptr<Graphics> graphics;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	class InputLayout {
	public:
		// クラスの新しいインスタンスを初期化
		InputLayout(
			ID3D11Device* graphicsDevice,
			const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
			const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength);

		~InputLayout() = default;

		ID3D11InputLayout* GetNativePointer();

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	};

	// スワップ チェーンを管理してレンダーターゲットの機能を提供するクラスを表します。
	class SwapChain final
	{
	public:
		// このクラスのインスタンスを初期化します。
		SwapChain(std::shared_ptr<Graphics> graphics, HWND hWnd, int width, int height);
		~SwapChain() = default;

		// ID3D11RenderTargetView を取得します。
		ID3D11RenderTargetView* GetRenderTargetView();
		// ID3D11DepthStencilView を取得します。
		ID3D11DepthStencilView* GetDepthStencilView();

		// バックバッファーに描画したイメージをディスプレイに表示します。
		void Present(UINT syncInterval);

	private:
		// 継承
		std::shared_ptr<Graphics> graphics;
		
		HWND window = nullptr;

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

	class Texture2D
	{
	public:
		Texture2D(
			ID3D11Device* graphicsDevice,
			UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);

		~Texture2D() = default;

		void SetData(const void* data);

		ID3D11Texture2D* GetNativePointer();
		ID3D11SamplerState* GetSamplerState();
		ID3D11ShaderResourceView* GetShaderResourceView();

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	};

	// アプリケーション全体を表します。
	class Game
	{
	public:
		Game(const ProjectSettings& settings);
		Game(const Game&) noexcept = delete;
		virtual ~Game() = default;

		void Initialize(HWND hWnd);
		void Update() noexcept;
		void Render() noexcept;
		void Release() noexcept;

		const std::wstring& GetTitle() const;
		int GetWidth() const;
		int GetHeight() const;

	protected:
		virtual void OnInitialize() {};
		virtual void OnUpdate() noexcept {}
		virtual void OnRender() noexcept {}
		virtual void OnRelease() noexcept {}

	private:
		std::wstring title = L"Game Title";
		int width = 640;
		int height = 480;

		// メインウィンドウ
		HWND window = nullptr;
		// グラフィックス機能
		std::shared_ptr<Graphics> graphics;
		// スワップチェーン
		std::unique_ptr<SwapChain> swapChain;

		// Direct3D 11のデバイス コンテキスト
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;

		// 画面クリアーに使用するカラー
		DirectX::XMVECTORF32 clearColor = { 0 / 255.0f, 99 / 255.0f, 181 / 255.0f, 1.0f };
		// ビューポート
		D3D11_VIEWPORT viewports[1] = {};

		// 定数バッファーを介してシェーダーに毎フレーム送る行列データを表します。
		struct MatricesPerFrame {
			DirectX::XMFLOAT4X4 worldMatrix;
			DirectX::XMFLOAT4X4 viewMatrix;
			DirectX::XMFLOAT4X4 projectionMatrix;
			DirectX::XMFLOAT4X4 worldViewProjectionMatrix;

			// カメラの位置座標
			DirectX::XMFLOAT4 viewPosition = DirectX::XMFLOAT4(0, 1, -10, 1);

			// ライトの位置座標(平行光源 w = 0, 点光源 w = 1)
			DirectX::XMFLOAT4 lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 1.0f);

			// マテリアルの表面カラー
			DirectX::XMFLOAT4 materialDiffuse = DirectX::XMFLOAT4(1, 1, 0, 1);

			// 鏡面反射の色(r, g, b) = (x, y, z)
			DirectX::XMFLOAT3 materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
			// 鏡面反射の強さ(float) = w
			float materialSpecularPower = 1;
		};

		// 自分で作ったクラスはunique or shared、既存のクラスはComptr
		// バッファー
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		// インデックスの数
		UINT indexCount = 0;
		// 定数バッファー
		std::unique_ptr<ConstantBuffer> constantBuffer;
		// シェーダー
		std::unique_ptr<BasicVertexShader> vertexShader;
		std::unique_ptr<BasicGeometryShader> geometryShader;
		std::unique_ptr<BasicPixelShader> pixelShader;
		// 入力レイアウト
		std::unique_ptr<InputLayout> inputLayout;
		// テクスチャー
		std::unique_ptr<Texture2D> texture;
	};

	class Application final
	{
	public:
		Application() noexcept = delete;
		Application(const Application&) noexcept = delete;
		~Application() = default;

		static int Run(Game* game, HINSTANCE hInstance, int nShowCmd) noexcept;
		static HWND GetWindowHandle() noexcept;
	};
}
