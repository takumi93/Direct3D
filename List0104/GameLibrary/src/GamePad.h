#pragma once

#include <GameLibrary/DigitalInput.h>
#include <GameLibrary/AnalogInput.h>

#include <Windows.h>
#include <Xinput.h>

namespace GameLibrary
{
	/// <summary>
	/// ゲームパッドを表します。
	/// </summary>
	class GamePad final
	{
	public:
		GamePad() noexcept = default;
		~GamePad() = default;

		void Initialize(int userIndex = 0);
		void Update(
			bool(&currentButtons)[DigitalInput::NumDigitalInputs],
			float(&analogs)[AnalogInput::NumAnalogInputs]) noexcept;

	private:
		int userIndex = 0;
		XINPUT_STATE state = {};
	};
}
