#include "Keyboard.h"
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

/// <summary>
/// このクラスのインスタンスを解放します。
/// </summary>
Keyboard::~Keyboard()
{
	if (device) {
		device->Unacquire();
		device.Reset();
	}
}

/// <summary>
/// このクラスのインスタンスを初期化します。
/// </summary>
/// <param name="directInput"></param>
/// <param name="hWnd">ウィンドウ ハンドル</param>
void Keyboard::Initialize(IDirectInput8W* directInput, HWND hWnd)
{
	ThrowIfFailed(directInput->CreateDevice(GUID_SysKeyboard, &device, nullptr));
	ThrowIfFailed(device->SetDataFormat(&c_dfDIKeyboard));
	ThrowIfFailed(device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	constexpr auto header = DIPROPHEADER{
		.dwSize = sizeof(DIPROPDWORD),
		.dwHeaderSize = sizeof(DIPROPHEADER),
		.dwObj = 0,
		.dwHow = DIPH_DEVICE,
	};
	ThrowIfFailed(device->SetProperty(DIPROP_BUFFERSIZE, &header));
}

/// <summary>
/// 現在のキー入力状態を更新します。
/// </summary>
void Keyboard::Update(bool(&currentButtons)[DigitalInput::NumDigitalInputs]) noexcept
{
	if (SUCCEEDED(device->Acquire())) {
		device->GetDeviceState(sizeof keyBuffer, keyBuffer);
	}

	for (size_t digitalInput = 0; digitalInput < DigitalInput::NumKeys; ++digitalInput) {
		currentButtons[digitalInput] = GetButton(static_cast<DigitalInput>(digitalInput));
	}
}

/// <summary>
/// 指定したボタン入力状態を取得します。
/// </summary>
/// <param name="digitalInput">Digital Input</param>
/// <returns>ボタンを押し下げている場合は true、離している場合は false</returns>
bool Keyboard::GetButton(DigitalInput digitalInput) const noexcept
{
	static constexpr unsigned char s_DIK_Mapping[DigitalInput::NumKeys] = {
		DIK_ESCAPE,
		DIK_1,
		DIK_2,
		DIK_3,
		DIK_4,
		DIK_5,
		DIK_6,
		DIK_7,
		DIK_8,
		DIK_9,
		DIK_0,
		DIK_MINUS,
		DIK_EQUALS,
		DIK_BACK,
		DIK_TAB,
		DIK_Q,
		DIK_W,
		DIK_E,
		DIK_R,
		DIK_T,
		DIK_Y,
		DIK_U,
		DIK_I,
		DIK_O,
		DIK_P,
		DIK_LBRACKET,
		DIK_RBRACKET,
		DIK_RETURN,
		DIK_LCONTROL,
		DIK_A,
		DIK_S,
		DIK_D,
		DIK_F,
		DIK_G,
		DIK_H,
		DIK_J,
		DIK_K,
		DIK_L,
		DIK_SEMICOLON,
		DIK_APOSTROPHE,
		DIK_GRAVE,
		DIK_LSHIFT,
		DIK_BACKSLASH,
		DIK_Z,
		DIK_X,
		DIK_C,
		DIK_V,
		DIK_B,
		DIK_N,
		DIK_M,
		DIK_COMMA,
		DIK_PERIOD,
		DIK_SLASH,
		DIK_RSHIFT,
		DIK_MULTIPLY,
		DIK_LMENU,
		DIK_SPACE,
		DIK_CAPITAL,
		DIK_F1,
		DIK_F2,
		DIK_F3,
		DIK_F4,
		DIK_F5,
		DIK_F6,
		DIK_F7,
		DIK_F8,
		DIK_F9,
		DIK_F10,
		DIK_NUMLOCK,
		DIK_SCROLL,
		DIK_NUMPAD7,
		DIK_NUMPAD8,
		DIK_NUMPAD9,
		DIK_SUBTRACT,
		DIK_NUMPAD4,
		DIK_NUMPAD5,
		DIK_NUMPAD6,
		DIK_ADD,
		DIK_NUMPAD1,
		DIK_NUMPAD2,
		DIK_NUMPAD3,
		DIK_NUMPAD0,
		DIK_DECIMAL,
		DIK_F11,
		DIK_F12,
		DIK_NUMPADENTER,
		DIK_RCONTROL,
		DIK_DIVIDE,
		DIK_SYSRQ,
		DIK_RMENU,
		DIK_PAUSE,
		DIK_HOME,
		DIK_UP,
		DIK_PRIOR,
		DIK_LEFT,
		DIK_RIGHT,
		DIK_END,
		DIK_DOWN,
		DIK_NEXT,
		DIK_INSERT,
		DIK_DELETE,
		DIK_LWIN,
		DIK_RWIN,
		DIK_APPS,
	};

	return (keyBuffer[s_DIK_Mapping[digitalInput]] & 0x80);
}