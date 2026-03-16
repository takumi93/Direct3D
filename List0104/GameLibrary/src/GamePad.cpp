#include "GamePad.h"
#include <GameLibrary/Utility.h>

#include <format>
#include <climits>

using namespace GameLibrary;

namespace
{
	/// <summary>
	/// コントローラーのスティックの入力範囲
	/// </summary>
	/// <param name="value">スティックの入力値</param>
	/// <param name="deadZone">デッドゾーンの値</param>
	/// <returns></returns>
	inline float NormalizeAnalogInput(SHORT value, SHORT deadZone)
	{
		if (value < 0) {
			if (value > -deadZone) {
				return 0.0f;
			}
			else {
				return (static_cast<int>(value) + deadZone) / static_cast<float>(- SHRT_MIN - deadZone);
			}
		}
		else {
			if (value < deadZone) {
				return 0.0f;
			}
			else {
				return (static_cast<int>(value) - deadZone) / static_cast<float>(SHRT_MAX - deadZone);
			}
		}
	}
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="userIndex"></param>
void GamePad::Initialize(int userIndex)
{
	this->userIndex = userIndex;
}

/// <summary>
/// 入力情報の更新
/// </summary>
/// <param name="currentButtons"></param>
/// <param name="analogs"></param>
void GamePad::Update(
	bool(&currentButtons)[DigitalInput::NumDigitalInputs],
	float(&analogs)[AnalogInput::NumAnalogInputs]) noexcept
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	const auto result = XInputGetState(userIndex, &state);
	if (result == ERROR_SUCCESS) {
		const auto& gamepad = state.Gamepad;
		const auto& buttons = gamepad.wButtons;
		// ボタン入力
		currentButtons[DigitalInput::DPadUp] = (buttons & XINPUT_GAMEPAD_DPAD_UP);
		currentButtons[DigitalInput::DPadDown] = (buttons & XINPUT_GAMEPAD_DPAD_DOWN);
		currentButtons[DigitalInput::DPadLeft] = (buttons & XINPUT_GAMEPAD_DPAD_LEFT);
		currentButtons[DigitalInput::DPadRight] = (buttons & XINPUT_GAMEPAD_DPAD_RIGHT);
		currentButtons[DigitalInput::StartButton] = (buttons & XINPUT_GAMEPAD_START);
		currentButtons[DigitalInput::BackButton] = (buttons & XINPUT_GAMEPAD_BACK);
		currentButtons[DigitalInput::LeftThumbClick] = (buttons & XINPUT_GAMEPAD_LEFT_THUMB);
		currentButtons[DigitalInput::RightThumbClick] = (buttons & XINPUT_GAMEPAD_RIGHT_THUMB);
		currentButtons[DigitalInput::LShoulder] = (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		currentButtons[DigitalInput::RShoulder] = (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
		currentButtons[DigitalInput::AButton] = (buttons & XINPUT_GAMEPAD_A);
		currentButtons[DigitalInput::BButton] = (buttons & XINPUT_GAMEPAD_B);
		currentButtons[DigitalInput::XButton] = (buttons & XINPUT_GAMEPAD_X);
		currentButtons[DigitalInput::YButton] = (buttons & XINPUT_GAMEPAD_Y);
		// アナログ入力
		analogs[AnalogInput::LeftTrigger] = gamepad.bLeftTrigger / 255.0f;
		analogs[AnalogInput::RightTrigger] = gamepad.bRightTrigger / 255.0f;
		analogs[AnalogInput::LeftStickX] = NormalizeAnalogInput(gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		analogs[AnalogInput::LeftStickY] = NormalizeAnalogInput(gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		analogs[AnalogInput::RightStickX] = NormalizeAnalogInput(gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		analogs[AnalogInput::RightStickY] = NormalizeAnalogInput(gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
}