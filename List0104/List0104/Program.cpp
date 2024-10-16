//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#include <Windows.h>

// アプリケーションのエントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウィンドウクラスの登録
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.lpfnWndProc = DefWindowProc;
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

	MessageBox(hWnd, L"ウィンドウが表示されましたか？", L"確認", MB_OK);

	return 0;
}
