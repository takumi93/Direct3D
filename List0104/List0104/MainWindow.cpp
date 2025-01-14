//=============================================================================
// MainWindow.cpp
// アプリケーションのメイン ウィンドウとなるクラスを提供します。
//=============================================================================
#include "Game.h"
#include <system_error>

// このクラスのインスタンスを初期化します。
MainWindow::MainWindow(const WindowSettings& settings)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// ウィンドウ クラスを登録する
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
		// 通知はするけど特に何もしない
		throw std::system_error(GetLastError(), std::system_category(), __FUNCTION__);
	}

	// クライアント領域が指定した解像度になるウィンドウサイズを計算
	RECT rect = { 0, 0, settings.screenWidth, settings.screenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウを作成する
	handle = CreateWindowEx(0,
		CLASS_NAME,// ウィンドウ クラス
		settings.title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	//handleの正常性確認
	if (handle == NULL) {
		throw std::system_error(GetLastError(), std::system_category(), __FUNCTION__);
	}

	//ウィンドウの表示と更新
	ShowWindow(handle, SW_SHOWNORMAL);
	UpdateWindow(handle);

	// この場合、thisを省略できないため記載
	this->settings = settings;
}

// このウィンドウの幅を取得します。
int MainWindow::GetWidth() const
{
	return settings.screenWidth;
}

// このウィンドウの高さを取得します。
int MainWindow::GetHeight() const
{
	return settings.screenHeight;
}

// このウィンドウのハンドルを取得します。
HWND MainWindow::GetHandle() const
{
	return handle;
}

// ウィンドウ メッセージを処理するプロシージャー
LRESULT CALLBACK MainWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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