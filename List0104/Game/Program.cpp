//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "SampleGame.h"

using namespace GameLibrary;

// アプリケーションのエントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const ProjectSettings settings = {
	.Title = L"Direct3D 11 サンプル",
	.Width = 1280,
	.Height = 720,
	};
	SampleGame game(settings);
	return Application::Run(&game, hInstance, nShowCmd);
}