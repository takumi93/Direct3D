#pragma once

#include <Windows.h>
#include <GameLibrary/DigitalInput.h>

#include <wrl/client.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace GameLibrary
{
	/// <summary>
	/// キーボード デバイスを表します。
	/// </summary>
	class Keyboard final
	{
	public:
		Keyboard() = default;
		~Keyboard();

		void Initialize(IDirectInput8W* directInput, HWND hWnd);
		void Update(bool(&currentButtons)[DigitalInput::NumDigitalInputs]) noexcept;

		bool GetButton(DigitalInput digitalInput) const noexcept;

	private:
		Microsoft::WRL::ComPtr<IDirectInputDevice8W> device;
		unsigned char keyBuffer[256] = {};
	};
}
