#include <GameLibrary/InputLayout.h>
#include <GameLibrary/Utility.h>
#include "InputSignature_SpriteVertex.h"

using namespace GameLibrary;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphicsDevice">グラフィックス デバイス</param>
/// <param name="inputElementDescs">頂点の入力要素配列へのポインター</param>
/// <param name="numElements">inputElementDescsの要素数</param>
/// <param name="shaderBytecodeWithInputSignature">入力署名が記述されているシェーダーのバイトデータ</param>
/// <param name="bytecodeLength">shaderBytecodeWithInputSignatureのサイズ</param>
InputLayout::InputLayout(ID3D11Device5* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreateInputLayout(
		inputElementDescs, numElements,
		shaderBytecodeWithInputSignature, bytecodeLength,
		&inputLayout));
}

/// <summary>
/// Direct3D 11 ネイティブのポインターを取得します。
/// </summary>
/// <returns>ID3D11InputLayout ポインター</returns>
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return inputLayout.Get();
}

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphicsDevice">グラフィックス デバイス</param>
InputLayout_Sprite::InputLayout_Sprite(ID3D11Device5* graphicsDevice)
	: InputLayout(graphicsDevice,
		Vertex_Sprite::InputElementDescs, Vertex_Sprite::NumElements,
		g_InputSignature_SpriteVertex, sizeof(g_InputSignature_SpriteVertex))
{

}
