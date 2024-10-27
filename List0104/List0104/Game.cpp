//=============================================================================
// Game.cpp
// 
//=============================================================================
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

// ウィンドウを作成。
bool Game::InitWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// ウィンドウ クラスを登録する
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;	// ウィンドウ プロシージャーを指定
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

	// ウィンドウを作成する
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

	// この場合は、thisキーワードを省略できない。
	this->hWnd = hWnd;

	return true;
}

// ウィンドウ メッセージを処理するプロシージャー
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// ウィンドウを閉じる
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

// メッセージループを実行
int Game::Run()
{
	// ウィンドウを作成
	if (!InitWindow()) {
		MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"メッセージ", MB_OK);
		return -1;
	}

	// メッセージループを実行
	MSG msg = {};
	while (true) {
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

	return 0;
}
