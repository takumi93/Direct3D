#pragma once

#include <GameLibrary/Graphics.h>

namespace GameLibrary
{
	class InputLayout : public GraphicsResource
	{
	protected:
		InputLayout(ID3D11Device5* graphicsDevice,
			const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
			const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength);

	public:
		ID3D11InputLayout* GetNativePointer();

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	};

	class InputLayout_Sprite final : public InputLayout
	{
	public:
		InputLayout_Sprite(ID3D11Device5* graphicsDevice);
	};
}
