//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>
#include <GameLibrary/Graphics.h>

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

	//// 頂点バッファーを表します。
	//class VertexBuffer
	//{
	//public:
	//	// このクラスの新しいインスタンスを初期化します。
	//	VertexBuffer(std::shared_ptr<Graphics> graphics, UINT byteWidth);

	//	~VertexBuffer() = default;

	//	// バッファーにデータを設定します。
	//	void SetData(const void* data);

	//	// D3D11のネイティブポインターを取得します。
	//	ID3D11Buffer* GetNativePointer();

	//private:
	//	std::shared_ptr<Graphics> graphics;
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	//	UINT stride = 0;
	//};

	//// インデックスバッファーを表します。
	//class IndexBuffer
	//{
	//public:
	//	// このクラスの新しいインスタンスを初期化します。
	//	IndexBuffer(std::shared_ptr<Graphics> graphics, UINT byteWidth);
	//	~IndexBuffer() = default;

	//	// バッファーにデータを設定します。
	//	void SetData(const void* data);

	//	// D3D11のネイティブポインターを取得します。
	//	ID3D11Buffer* GetNativePointer();

	//private:
	//	std::shared_ptr<Graphics> graphics;
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	//};

	//// 定数バッファーを表します。
	//class ConstantBuffer
	//{
	//public:
	//	// このクラスの新しいインスタンスを初期化します。
	//	ConstantBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth);
	//	~ConstantBuffer() = default;

	//	// バッファーにデータを設定します。
	//	void SetData(const void* data);

	//	// D3D11のネイティブポインターを取得します。
	//	ID3D11Buffer* GetNativePointer();

	//private:
	//	std::shared_ptr<Graphics> graphics;
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	//};

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

		// メインウィンドウ
		HWND window = nullptr;
		// グラフィックス機能
		std::shared_ptr<Graphics> graphics;
		// スワップチェーン
		std::unique_ptr<SwapChain> swapChain;

		// Direct3D 11のデバイス コンテキスト
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> deviceContext;

	private:
		std::wstring title = L"Game Title";
		int width = 640;
		int height = 480;
		bool forceVSync = false;
		bool useWarpAdapter = false;

		// 画面クリアーに使用するカラー
		DirectX::XMVECTORF32 clearColor = { 0 / 255.0f, 99 / 255.0f, 181 / 255.0f, 1.0f };

		// ビューポート
		D3D11_VIEWPORT viewport = {
			.TopLeftX = 0,
			.TopLeftY = 0,
			.Width = 0,
			.Height = 0,
			.MinDepth = 0.0f,
			.MaxDepth = 1.0f,
		};

		//// 定数バッファーを介してシェーダーに毎フレーム送る行列データを表します。
		//struct MatricesPerFrame {
		//	DirectX::XMFLOAT4X4 worldMatrix;
		//	DirectX::XMFLOAT4X4 viewMatrix;
		//	DirectX::XMFLOAT4X4 projectionMatrix;
		//	DirectX::XMFLOAT4X4 worldViewProjectionMatrix;

		//	// カメラの位置座標
		//	DirectX::XMFLOAT4 viewPosition = DirectX::XMFLOAT4(0, 1, -10, 1);

		//	// ライトの位置座標(平行光源 w = 0, 点光源 w = 1)
		//	DirectX::XMFLOAT4 lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 1.0f);

		//	// マテリアルの表面カラー
		//	DirectX::XMFLOAT4 materialDiffuse = DirectX::XMFLOAT4(1, 1, 0, 1);

		//	// 鏡面反射の色(r, g, b) = (x, y, z)
		//	DirectX::XMFLOAT3 materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
		//	// 鏡面反射の強さ(float) = w
		//	float materialSpecularPower = 1;
		//};

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
		//// テクスチャー
		//std::unique_ptr<Texture2D> texture;
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
