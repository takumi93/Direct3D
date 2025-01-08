//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <d3d11.h>
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

class MainWindow {
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
	// 関数のプロトタイプ宣言
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	WindowSettings settings;
	// ウィンドウのハンドル
	HWND handle = NULL;
};

// アプリケーション全体を表します。
class Game {
public:
	//// このクラスのインスタンスを初期化します。
	//Game() noexcept = default;
	//virtual ~Game() = default;

	// メッセージループを実行します。
	int Run(const WindowSettings& settings = WindowSettings()) ;

private:
	// メインウィンドウ
	std::unique_ptr<MainWindow> window;

	// DXGI 1.1のファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	// DXGI 1.1のアダプター
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
	// DXGI 1.1のデバイス
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
	// Direct3D 11のデバイス(パソコンのグラフィック機能そのもの、GPU)
	Microsoft::WRL::ComPtr<ID3D11Device> graphicsDevice;
	// Direct3D 11のデバイス コンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	// レンダーターゲット
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> renderTargetResourceView;
	// 深度ステンシルのフォーマット
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 深度ステンシル
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	// 深度ステンシルをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthStencilResourceView;
	// 画面クリアーに使用するカラー
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};

	// グラフィックデバイスを作成。Game.cpp参照
	bool InitGraphicsDevice();
};