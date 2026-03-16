#pragma once

#include <GameLibrary/DigitalInput.h>
#include <GameLibrary/AnalogInput.h>

#include <Windows.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	/// <summary>
	/// ユーザー入力を取得する機能を提供します。
	/// </summary>
	class Input final
	{
	public:
		static void Initialize(HINSTANCE hInstance, HWND hWnd);
		static void Shutdown() noexcept;
		static void Update() noexcept;

		static bool GetButtonDown(DigitalInput digitalInput) noexcept;
		static bool GetButton(DigitalInput digitalInput) noexcept;
		static bool GetButtonUp(DigitalInput digitalInput) noexcept;
		static float GetAxis(AnalogInput analogInput) noexcept;
		static const DirectX::XMFLOAT2& GetMousePosition() noexcept;
	};
}