#pragma once

#include <Windows.h>
#include <GameLibrary/DigitalInput.h>
#include <GameLibrary/AnalogInput.h>

#include <wrl/client.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	/// <summary>
	/// マウス デバイスを表します。
	/// </summary>
	class Mouse final
	{
	public:
		Mouse() noexcept = default;
		~Mouse();

		void Initialize(IDirectInput8W* directInput, HWND hWnd);
		void Update(
			bool(&currentButtons)[DigitalInput::NumDigitalInputs],
			float(&analogs)[AnalogInput::NumAnalogInputs]) noexcept;

		bool GetButton(DigitalInput digitalInput) const noexcept;
		LONG GetMouseX() const noexcept;
		LONG GetMouseY() const noexcept;
		LONG GetMouseScroll() const noexcept;
		const DirectX::XMFLOAT2& GetMousePosition() const noexcept;

	private:
		void UpdateMousePosition() noexcept;

		HWND hWnd = NULL;
		Microsoft::WRL::ComPtr<IDirectInputDevice8W> device;
		DIMOUSESTATE2 mouseState = {};
		DirectX::XMFLOAT2 mousePosition = {};
	};
}
