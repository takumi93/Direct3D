//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#include <Windows.h>

// 関数のプロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// アプリケーションのエントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウィンドウクラスの登録
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.lpfnWndProc = WindowProc;	// ウィンドウ プロシージャーを指定
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszClassName = CLASS_NAME;
	//ウィンドウクラスの登録確認
	if (!RegisterClassExW(&wndClass)) {
		MessageBox(NULL, L"ウィンドウクラスの登録に失敗しました。", L"エラー", MB_OK);
		return 0;
	}

	//ウィンドウハンドル
	HWND hWnd = NULL;

	//ウィンドウの大きさ計算
	RECT rect = { 0, 0, 640, 480 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	//ウィンドウの作成
	hWnd = CreateWindowEx(
		0, CLASS_NAME, L"タイトル", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), //ウィンドウ幅計算
		NULL, NULL, hInstance, NULL
	);
	//hWndの正常性確認
	if (hWnd == NULL) {
		MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"エラー", MB_OK);
		return 0;
	}

	//ウィンドウの表示と更新
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = {};
	while (true) {
		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessageW(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return 0;
}

// ウィンドウ・メッセージを処理するプロシージャー
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// ウィンドウを閉じる
		if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"情報", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		// アプリケーションを終了
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		// 参考：Windows APIによる図形描画
		PAINTSTRUCT ps;
		auto hdc = BeginPaint(hWnd, &ps);
		RECT rect = {};
		rect.left = 200;
		rect.top = 100;
		rect.right = 300;
		rect.bottom = 200;
		FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

		rect.left = 400;
		rect.top = 100;
		rect.right = 600;
		rect.bottom = 200;
		FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 2));
		EndPaint(hWnd, &ps);
	}
	break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
