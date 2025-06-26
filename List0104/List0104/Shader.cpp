//=============================================================================
// Shaders.cpp
// 頂点、ジオメトリ、ピクセル他のシェーダーを管理する機能が含まれます。
//=============================================================================
#include "Game.h"
#include <comdef.h>
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"

using namespace Microsoft::WRL;

// このクラスの新しいインスタンスを初期化します。
BasicVertexShader::BasicVertexShader(std::shared_ptr<Graphics> graphics)
{
	ComPtr<ID3D11VertexShader> shader;
	const auto hr = graphics->GetDevice()->CreateVertexShader(
		g_BasicVertexShader, ARRAYSIZE(g_BasicVertexShader), NULL,
		&shader);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->shader = shader;
}

// このシェーダーのバイトコードを取得します。
const BYTE* BasicVertexShader::GetBytecode()
{
	return g_BasicVertexShader;
}

// バイトコードのサイズを取得します。
size_t BasicVertexShader::GetBytecodeLength()
{
	return sizeof g_BasicVertexShader;
}

// D3D11のネイティブポインターを取得します。
ID3D11VertexShader* BasicVertexShader::GetNativePointer()
{
	return shader.Get();
}

// このクラスの新しいインスタンスを初期化します。
BasicGeometryShader::BasicGeometryShader(std::shared_ptr<Graphics> graphics)
{
	ComPtr<ID3D11GeometryShader> shader;
	const auto hr = graphics->GetDevice()->CreateGeometryShader(
		g_BasicGeometryShader, ARRAYSIZE(g_BasicGeometryShader), NULL,
		&shader);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->shader = shader;
}

// このシェーダーのバイトコードを取得します。
const BYTE* BasicGeometryShader::GetBytecode()
{
	return g_BasicGeometryShader;
}

// バイトコードのサイズを取得します。
size_t BasicGeometryShader::GetBytecodeLength()
{
	return ARRAYSIZE(g_BasicGeometryShader);
}

// D3D11のネイティブポインターを取得します。
ID3D11GeometryShader* BasicGeometryShader::GetNativePointer()
{
	return shader.Get();
}

// このクラスの新しいインスタンスを初期化します。
BasicPixelShader::BasicPixelShader(std::shared_ptr<Graphics> graphics)
{
	ComPtr<ID3D11PixelShader> shader;
	const auto hr = graphics->GetDevice()->CreatePixelShader(
		g_BasicPixelShader, ARRAYSIZE(g_BasicPixelShader), NULL,
		&shader);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->shader = shader;
}

// このシェーダーのバイトコードを取得します。
const BYTE* BasicPixelShader::GetBytecode()
{
	return g_BasicPixelShader;
}

// バイトコードのサイズを取得します。
size_t BasicPixelShader::GetBytecodeLength()
{
	return sizeof g_BasicPixelShader;
}

// D3D11のネイティブポインターを取得します。
ID3D11PixelShader* BasicPixelShader::GetNativePointer()
{
	return shader.Get();
}