#pragma once

#include <GameLibrary/DigitalInput.h>
#include <GameLibrary/AnalogInput.h>
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"

#include <Windows.h>
#include <wrl/client.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	/// <summary>
	/// ユーザー入力を管理します。
	/// </summary>
	class InputManager final
	{
	public:
		InputManager(HINSTANCE hInstance, HWND hWnd);
		~InputManager() = default;

		void Update() noexcept;

		bool GetButtonDown(DigitalInput digitalInput) const noexcept;
		bool GetButton(DigitalInput digitalInput) const noexcept;
		bool GetButtonUp(DigitalInput digitalInput) const noexcept;
		float GetAxis(AnalogInput analogInput) const noexcept;
		const DirectX::XMFLOAT2& GetMousePosition() const noexcept;

	private:
		// ボタン入力状態
		bool buttons[2][DigitalInput::NumDigitalInputs] = {};
		// 現在のフレームでのボタン入力状態
		bool (*currentButtons)[DigitalInput::NumDigitalInputs] = nullptr;
		// 直前のフレームでのボタン入力状態
		bool (*lastButtons)[DigitalInput::NumDigitalInputs] = nullptr;
		// 現在のフレームでのアナログ入力状態
		float analogs[AnalogInput::NumAnalogInputs] = {};

		Microsoft::WRL::ComPtr<IDirectInput8W> directInput;
		Keyboard keyboard;
		Mouse mouse;
		GamePad gamePad;
	};
}
