#include <GameLibrary/Input.h>
#include "InputManager.h"

#include <memory>

using namespace GameLibrary;
using namespace DirectX;

namespace
{
	std::unique_ptr<InputManager> s_Manager;
}

/// <summary>
/// ユーザー入力機能を初期化します。
/// </summary>
/// <param name="hInstance">インスタンス ハンドル</param>
/// <param name="hWnd">ウィンドウ ハンドル</param>
void Input::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	s_Manager = std::make_unique<InputManager>(hInstance, hWnd);
}

/// <summary>
/// ユーザー入力機能を解放します。
/// </summary>
void Input::Shutdown() noexcept
{
	s_Manager.reset();
}

/// <summary>
/// 現在のフレームでのユーザー入力を更新します。
/// </summary>
void Input::Update() noexcept
{
	s_Manager->Update();
}

/// <summary>
/// 現在のフレームでボタン入力が開始されたかを取得します。
/// </summary>
/// <param name="digitalInput">デジタル入力の種類</param>
/// <returns>ボタン入力が開始された場合は true、それ以外は false</returns>
bool Input::GetButtonDown(DigitalInput digitalInput) noexcept
{
	return s_Manager->GetButtonDown(digitalInput);
}

/// <summary>
/// 現在のボタン入力状態を取得します。
/// </summary>
/// <param name="digitalInput">デジタル入力の種類</param>
/// <returns>押し下げられている場合は true、それ以外は false</returns>
bool Input::GetButton(DigitalInput digitalInput) noexcept
{
	return s_Manager->GetButton(digitalInput);
}

/// <summary>
/// 現在のフレームでボタン入力が終了されたかを取得します。
/// </summary>
/// <param name="digitalInput">デジタル入力の種類</param>
/// <returns>ボタン入力が終了された場合は true、それ以外は false</returns>
bool Input::GetButtonUp(DigitalInput digitalInput) noexcept
{
	return s_Manager->GetButtonUp(digitalInput);
}

/// <summary>
/// アナログ入力値を取得します。
/// </summary>
/// <param name="analogInput">アナログ入力の種類</param>
/// <returns>float値</returns>
float Input::GetAxis(AnalogInput analogInput) noexcept
{
	return s_Manager->GetAxis(analogInput);
}

/// <summary>
/// マウスのカーソル位置を取得します。
/// </summary>
/// <returns>位置座標</returns>
const XMFLOAT2& Input::GetMousePosition() noexcept
{
	return s_Manager->GetMousePosition();
}