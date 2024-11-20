//=============================================================================
// Game.cpp
// 
//=============================================================================
//#include <d3d11.h> Game.hにこの機能を使用するコードを移動したため
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <DirectXColors.h>	// DirectXのカラーライブラリー
#include <iterator>
#include "Game.h"

// 関数のプロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 初期値で上書きしてアプリケーションを初期化します。
void Game::Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	// ウィンドウのタイトル
	//this->WindowTitle = windowTitle;
	
	// thisキーワードは省略できる。
	WindowTitle = windowTitle;
	// ウィンドウの幅
	ScreenWidth = screenWidth;
	// ウィンドウの高さ
	ScreenHeight = screenHeight;
}

// ウィンドウを作成します。
bool Game::InitWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// ウィンドウ クラスを登録
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	// ウィンドウ プロシージャーを指定
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszClassName = CLASS_NAME;
	//ウィンドウクラスの登録確認
	if (!RegisterClassEx(&wndClass)) {
		return 0;
	}

	// クライアント領域が指定した解像度になるウィンドウサイズを計算
	RECT rect = { 0, 0, ScreenWidth, ScreenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウを作成
	auto hWnd = CreateWindowEx(0,
		CLASS_NAME,	// ウィンドウ クラス
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	//hWndの正常性確認
	if (hWnd == NULL) {
		return false;
	}

	//ウィンドウの表示と更新
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	// この場合、thisを省略できないため記載
	this->hWnd = hWnd;

	return true;
}

// ウィンドウ メッセージを処理するプロシージャー
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// ウィンドウを閉じたとき確認用のメッセージボックスを表示
		if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"メッセージ", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		// アプリケーションを終了
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// グラフィックデバイスを作成します。
bool Game::InitGraphicsDevice()
{
	// 関数の実行結果を受け取る変数
	HRESULT hr = S_OK;

	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 作成するスワップチェーンについての情報を格納
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = ScreenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;
	swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT |
		DXGI_USAGE_SHADER_INPUT;	// シェーダーリソースとして使用することを設定
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Windowed = TRUE;

	// デバイス、デバイスコンテキスト、スワップチェーンを作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&swapChain, &graphicsDevice, &featureLevel, &immediateContext);
	if (FAILED(hr)) {
		return 0;
	}

	// バックバッファーを取得
	ID3D11Texture2D* backBuffer = nullptr;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	//失敗したら処理しない
	if (FAILED(hr)) {
		return 0;
	}
	// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
	hr = graphicsDevice->CreateRenderTargetView(backBuffer, NULL, &renderTargetViews[0]);
	//失敗したら処理しない
	if (FAILED(hr)) {
		return 0;
	}
	// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
	hr = graphicsDevice->CreateShaderResourceView(
		backBuffer,
		NULL,
		&renderTargetResourceView);
	//失敗したら処理しない
	if (FAILED(hr)) {
		return 0;
	}
	SAFE_RELEASE(backBuffer);

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
	ID3D11Texture2D* depthStencil = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
	depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = textureFormat;
	depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;	// シェーダーリソースとして使用することを設定
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	//失敗したら処理しない
	if (FAILED(hr)) {
		return 0;
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
	hr = graphicsDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView);
	//失敗したら処理しない
	if (FAILED(hr)) {
		return 0;
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
	hr = graphicsDevice->CreateShaderResourceView(
		depthStencil,
		&depthStencilResourceViewDesc,
		&depthStencilResourceView);
	//失敗したら処理しない
	if (FAILED(hr)) {
		return 0;
	}
	SAFE_RELEASE(depthStencil);

	// ビューポートを指定
	viewports[0].Width = static_cast<FLOAT>(ScreenWidth);
	viewports[0].Height = static_cast<FLOAT>(ScreenHeight);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;
}

// 使用したグラフィックリソースを解放。
void Game::ReleaseGraphicsDevice()
{
	SAFE_RELEASE(depthStencilResourceView);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(renderTargetResourceView);
	SAFE_RELEASE(renderTargetViews[0]);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}

// メッセージループを実行
int Game::Run()
{
	// ウィンドウを作成
	if (!InitWindow()) {
		MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"メッセージ", MB_OK);
		return -1;
	}
	// グラフィックデバイスを作成
	if (!InitGraphicsDevice()) {
		MessageBox(NULL, L"グラフィックデバイスを初期化できませんでした。", L"メッセージ", MB_OK);
		return -1;
	}

	HRESULT hr = S_OK;

	// 一つの頂点に含まれるデータの型
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// 位置座標
	};
	// 頂点データの配列
	VertexPosition vertices[] = {
		{ { -1.0f, 0.0f, 0.0f }, },
		{ {  0.0f, 1.0f, 0.0f }, },
		{ {  1.0f, 0.0f, 0.0f }, },
	};

	// 作成する頂点バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof vertices;	// 作成するバッファーのサイズ(bytes) x:4bytes,y:4bytes,z:4bytes total 12bytes
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// バッファーの使用方法（とりあえずDEFAULT）
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// Vertex Bufferとして利用する
	bufferDesc.CPUAccessFlags = 0;			// CPUからの読み書きに使わない場合は0
	bufferDesc.MiscFlags = 0;				// オプションのフラグ
	bufferDesc.StructureByteStride = 0;		// 頂点バッファーとして使うなら0
	// バッファーを作成※バッファー＝データを一時的に保存する場所
	ID3D11Buffer* vertexBuffer = nullptr;
	hr = graphicsDevice->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);
	if (FAILED(hr) || vertexBuffer == nullptr) {
		OutputDebugString(L"頂点バッファーを作成できませんでした。");
		return 0;
	}
	// バッファーにデータを転送
	immediateContext->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		// レンダーターゲットを設定
		immediateContext->OMSetRenderTargets(1, renderTargetViews, depthStencilView);
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(renderTargetViews[0], clearColor);
		immediateContext->ClearDepthStencilView(depthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// ビューポートを設定
		immediateContext->RSSetViewports(1, viewports);


		// 頂点バッファーを設定
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer };
		UINT strides[1] = { sizeof(VertexPosition) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(
			0,
			std::size(vertexBuffers),
			vertexBuffers, strides, offsets);


		// バックバッファーに描画したイメージをディスプレイに表示
		HRESULT hr = S_OK;
		hr = swapChain->Present(1, 0);
		if (FAILED(hr))
		{
			MessageBox(hWnd,
				L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
				L"エラー", MB_OK);
			return -1;
		}

		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	SAFE_RELEASE(vertexBuffer);

	ReleaseGraphicsDevice();

	return (int)msg.wParam;
}
