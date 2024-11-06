//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>

#define SAFE_RELEASE(p) if ((p) != nullptr) { (p)->Release(); (p) = nullptr; }

// アプリケーション全体を表します。
class Game {
	//private:	
	// クラスでアクセス指定子を省略するとprivateになります。
	
	// ウィンドウのタイトル
	LPCWSTR WindowTitle = L"タイトル";
	// ウィンドウの幅
	int ScreenWidth = 640;
	// ウィンドウの高さ
	int ScreenHeight = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;

public:
	// 必要な情報を上書きすることでアプリケーションを初期化します。
	void Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// メッセージループを実行します。
	int Run();

private:
	bool InitWindow();
};