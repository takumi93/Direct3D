//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#include <Windows.h>

//アプリケーションのエントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow) 
{
	//ウィンドウハンドル
	HWND hWnd = NULL;

	//ウィンドウの作成
	hWnd = CreateWindowExW(
		0, L"Static", L"サンプル", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL);

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