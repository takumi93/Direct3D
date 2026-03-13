//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include <GameLibrary/Game.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphicsDevice"></param>
/// <param name="inputElementDescs"></param>
/// <param name="numElements"></param>
/// <param name="shaderBytecodeWithInputSignature"></param>
/// <param name="bytecodeLength"></param>
InputLayout::InputLayout(
	ID3D11Device* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength)
{
	// 入力レイアウトを作成
	const auto hr = graphicsDevice->CreateInputLayout(
		inputElementDescs, numElements,
		shaderBytecodeWithInputSignature, bytecodeLength,
		&inputLayout);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->inputLayout = inputLayout;
}

///// <summary>
///// InputLayoutのリソースを解放します。
///// </summary>
//InputLayout::~InputLayout()
//{
//
//}

/// <summary>
/// D3D11のネイティブポインターを取得します。
/// </summary>
/// <returns></returns>
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return inputLayout.Get();
}