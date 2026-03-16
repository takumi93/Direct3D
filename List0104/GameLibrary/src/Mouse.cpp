#include "Mouse.h"
#include <GameLibrary/Utility.h>

using namespace GameLibrary;
using namespace DirectX;

/// <summary>
/// このクラスのインスタンスを解放します。
/// </summary>
Mouse::~Mouse()
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
void Mouse::Initialize(IDirectInput8W* directInput, HWND hWnd)
{
	this->hWnd = hWnd;
	ThrowIfFailed(directInput->CreateDevice(GUID_SysMouse, &device, nullptr));
	ThrowIfFailed(device->SetDataFormat(&c_dfDIMouse2));
	ThrowIfFailed(device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	UpdateMousePosition();
}

/// <summary>
/// 現在のマウス入力状態を更新します。
/// </summary>
void Mouse::Update(
	bool(&currentButtons)[DigitalInput::NumDigitalInputs],
	float(&analogs)[AnalogInput::NumAnalogInputs]) noexcept
{
	if (SUCCEEDED(device->Acquire())) {
		device->GetDeviceState(sizeof mouseState, &mouseState);
	}

	for (size_t digitalInput = DigitalInput::Mouse0; digitalInput <= DigitalInput::Mouse7; ++digitalInput) {
		currentButtons[digitalInput] = GetButton(static_cast<DigitalInput>(digitalInput));
	}

	analogs[AnalogInput::MouseX] = GetMouseX() * 0.018f;
	analogs[AnalogInput::MouseY] = GetMouseY() * -0.018f;

	if (GetMouseScroll() > 0) {
		analogs[AnalogInput::MouseScroll] = 1;
	}
	else if (GetMouseScroll() < 0) {
		analogs[AnalogInput::MouseScroll] = -1;
	}
	else {
		analogs[AnalogInput::MouseScroll] = 0;
	}

	UpdateMousePosition();
}

/// <summary>
/// マウスのカーソル位置を更新します。
/// </summary>
/// <param name="hWnd">ウィンドウのハンドル</param>
void Mouse::UpdateMousePosition() noexcept
{
	if (!IsWindowEnabled(hWnd)) {
		return;
	}

	auto point = POINT{};
	if (!GetCursorPos(&point)) {
		OutputLastError();
		return;
	}
	if (!ScreenToClient(hWnd, &point)) {
		OutputLastError();
		return;
	}
	mousePosition.x = static_cast<float>(point.x);
	mousePosition.y = static_cast<float>(point.y);
}

bool Mouse::GetButton(DigitalInput digitalInput) const noexcept
{
	return (mouseState.rgbButtons[digitalInput - DigitalInput::Mouse0] > 0);
}

LONG Mouse::GetMouseX() const noexcept
{
	return mouseState.lX;
}

LONG Mouse::GetMouseY() const noexcept
{
	return mouseState.lY;
}

LONG Mouse::GetMouseScroll() const noexcept
{
	return mouseState.lZ;
}

/// <summary>
/// マウスのカーソル位置を取得します。
/// </summary>
/// <returns>位置座標</returns>
const XMFLOAT2& Mouse::GetMousePosition() const noexcept
{
	return mousePosition;
}
