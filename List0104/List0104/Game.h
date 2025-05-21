//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>
#include <memory>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

struct WindowSettings
{
	// ウィンドウのタイトル
	std::wstring title = L"タイトル";
	// ウィンドウの幅
	int screenWidth = 640;
	// ウィンドウの高さ
	int screenHeight = 480;
};

// メイン ウィンドウを表します。
class MainWindow
{
public:
	// このクラスのインスタンスを初期化します。
	MainWindow(const WindowSettings& settings = WindowSettings());
	virtual ~MainWindow() = default;

	// このウィンドウの幅を取得します。
	int GetWidth() const;
	// このウィンドウの高さを取得します。
	int GetHeight() const;
	// このウィンドウのハンドルを取得します。
	HWND GetHandle() const;

private:
	// このウィンドウのメッセージを処理します。
	static LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	WindowSettings settings;
	// ウィンドウのハンドル
	HWND handle = NULL;
};

// グラフィックス機能を表します。
class Graphics final
{
public:
	// このクラスのインスタンスを初期化します。
	Graphics();
	~Graphics() = default;

	// IDXGIFactory1 を取得します。
	IDXGIFactory1* GetDXGI_Factory();
	// ID3D11Device を取得します。
	ID3D11Device* GetDevice();
	// ID3D11DeviceContext を取得します。
	ID3D11DeviceContext* GetDeviceContext();

private:
	// DXGI 1.1のファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	// DXGI 1.1のアダプター
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
	// DXGI 1.1のデバイス
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
	// Direct3D 11のデバイス
	Microsoft::WRL::ComPtr<ID3D11Device> graphicsDevice;
	// Direct3D 11のデバイス コンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
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

// 頂点バッファーを表します。
class VertexBuffer
{
public:
	// このクラスの新しいインスタンスを初期化します。
	VertexBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth);
	~VertexBuffer() = default;

	// バッファーにデータを設定します。
	void SetData(const void* data);

	// D3D11のネイティブポインターを取得します。
	ID3D11Buffer* GetNativePointer();

private:
	std::shared_ptr<Graphics> graphics;
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};

// インデックスバッファーを表します。
class IndexBuffer
{
public:
	// このクラスの新しいインスタンスを初期化します。
	IndexBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth);
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

	// このシェーダーのバイトコードを取得します。
	const BYTE* GetBytecode();
	// バイトコードのサイズを取得します。
	size_t GetBytecodeLength();

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

	// このシェーダーのバイトコードを取得します。
	const BYTE* GetBytecode();
	// バイトコードのサイズを取得します。
	size_t GetBytecodeLength();

	// D3D11のネイティブポインターを取得します。
	ID3D11PixelShader* GetNativePointer();

private:
	std::shared_ptr<Graphics> graphics;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
};

// スワップ チェーンを管理してレンダーターゲットの機能を提供するクラスを表します。
class SwapChain final
{
public:
	// このクラスのインスタンスを初期化します。
	SwapChain(std::shared_ptr<Graphics> graphics, std::shared_ptr<MainWindow> window,
		DXGI_FORMAT swapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
	~SwapChain() = default;

	// ID3D11RenderTargetView を取得します。
	ID3D11RenderTargetView* GetRenderTargetView();
	// ID3D11DepthStencilView を取得します。
	ID3D11DepthStencilView* GetDepthStencilView();

	// バックバッファーに描画したイメージをディスプレイに表示します。
	void Present(UINT syncInterval);

private:
	std::shared_ptr<Graphics> graphics;
	std::shared_ptr<MainWindow> window;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> renderTargetResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthStencilResourceView;
};

// アプリケーション全体を表します。
class Game
{
public:
	// メッセージループを実行します。
	int Run(const WindowSettings& settings = WindowSettings());

private:
	// メインウィンドウ
	std::shared_ptr<MainWindow> window;
	// グラフィックス機能
	std::shared_ptr<Graphics> graphics;
	// スワップチェーン
	std::unique_ptr<SwapChain> swapChain;

	// 画面クリアーに使用するカラー
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};
};
