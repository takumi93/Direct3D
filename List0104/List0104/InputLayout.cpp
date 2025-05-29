//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "Game.h"
#include <comdef.h>

InputLayout::InputLayout(
	ID3D11Device* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength)
{
	const auto hr = graphicsDevice->CreateInputLayout(
		inputElementDescs, numElements,
		shaderBytecodeWithInputSignature, bytecodeLength,
		&inputLayout);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->inputLayout = inputLayout;
}

//InputLayout::~InputLayout()
//{
//
//}

// D3D11のネイティブポインターを取得します。
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return inputLayout.Get();
}