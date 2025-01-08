//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <d3d11.h>

#define SAFE_RELEASE(p) if ((p) != nullptr) { (p)->Release(); (p) = nullptr; }

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
	IDXGIFactory1* dxgiFactory = nullptr;
	// DXGI 1.1のアダプター
	IDXGIAdapter1* dxgiAdapter = nullptr;
	// DXGI 1.1のデバイス
	IDXGIDevice1* dxgiDevice = nullptr;
	// Direct3D 11のデバイス(パソコンのグラフィック機能そのもの、GPU)
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11のデバイス コンテキスト
	ID3D11DeviceContext* immediateContext = nullptr;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// スワップチェーン
	IDXGISwapChain* swapChain = nullptr;
	// レンダーターゲット
	ID3D11RenderTargetView* renderTargetViews[1];
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	ID3D11ShaderResourceView* renderTargetResourceView = nullptr;
	// 深度ステンシルのフォーマット
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 深度ステンシル
	ID3D11DepthStencilView* depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソース ビュー
	ID3D11ShaderResourceView* depthStencilResourceView = nullptr;
	// 画面クリアーに使用するカラー
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};

	// グラフィックデバイスを作成。Game.cpp参照
	bool InitGraphicsDevice();
	// グラフィックリソースを解放します。Game.cpp参照
	void ReleaseGraphicsDevice();
};