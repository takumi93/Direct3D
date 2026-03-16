#pragma once

namespace GameLibrary
{
	enum AnalogInput : unsigned char
	{
		// ゲームパッド
		LeftTrigger,
		RightTrigger,
		LeftStickX,
		LeftStickY,
		RightStickX,
		RightStickY,

		// マウス
		MouseX,
		MouseY,
		MouseScroll,

		NumAnalogInputs
	};
}