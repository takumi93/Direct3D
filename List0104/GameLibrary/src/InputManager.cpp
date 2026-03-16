#include "InputManager.h"
#include <GameLibrary/Utility.h>

#include <format>

using namespace GameLibrary;
using namespace DirectX;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
InputManager::InputManager(HINSTANCE hInstance, HWND hWnd)
{
	currentButtons = &buttons[0];
	lastButtons = &buttons[1];

	ThrowIfFailed(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8W, &directInput, NULL));
	keyboard.Initialize(directInput.Get(), hWnd);
	mouse.Initialize(directInput.Get(), hWnd);
	gamePad.Initialize();
}

/// <summary>
/// フレーム更新を開始します。
/// </summary>
void InputManager::Update() noexcept
{
	// 前回フレームの状態を更新
	std::swap(lastButtons, currentButtons);

	keyboard.Update(*currentButtons);
	mouse.Update(*currentButtons, analogs);
	gamePad.Update(*currentButtons, analogs);
}

/// 現在のフレームでボタン入力が開始されたかを取得します。
/// </summary>
/// <param name="digitalInput">デジタル入力の種類</param>
/// <returns>ボタン入力が開始された場合は true、それ以外は false</returns>
bool InputManager::GetButtonDown(DigitalInput digitalInput) const noexcept
{
	return (!(*lastButtons)[digitalInput] && (*currentButtons)[digitalInput]);
}

/// <summary>
/// 現在のボタン入力状態を取得します。
/// </summary>
/// <param name="digitalInput">デジタル入力の種類</param>
/// <returns>押し下げられている場合は true、それ以外は false</returns>
bool InputManager::GetButton(DigitalInput digitalInput) const noexcept
{
	return (*currentButtons)[digitalInput];
}

/// <summary>
/// 現在のフレームでボタン入力が終了されたかを取得します。
/// </summary>
/// <param name="digitalInput">デジタル入力の種類</param>
/// <returns>ボタン入力が終了された場合は true、それ以外は false</returns>
bool InputManager::GetButtonUp(DigitalInput digitalInput) const noexcept
{
	return ((*lastButtons)[digitalInput] && !(*currentButtons)[digitalInput]);
}

/// <summary>
/// アナログ入力値を取得します。
/// </summary>
/// <param name="analogInput">アナログ入力の種類</param>
/// <returns>float値</returns>
float InputManager::GetAxis(AnalogInput analogInput) const noexcept
{
	return analogs[analogInput];
}

/// <summary>
/// マウスのカーソル位置を取得します。
/// </summary>
/// <returns>位置座標</returns>
const XMFLOAT2& InputManager::GetMousePosition() const noexcept
{
	return mouse.GetMousePosition();
}