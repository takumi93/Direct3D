//=============================================================================
// Shaders.cpp
// 頂点、ジオメトリ、ピクセル他のシェーダーを管理する機能が含まれます。
//=============================================================================
#include <GameLibrary/Game.h>
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"
#include <comdef.h>

using namespace GameLibrary;
using namespace Microsoft::WRL;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphics"></param>
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

///// <summary>
///// 
///// </summary>
//BasicVertexShader::~BasicVertexShader()
//{
//
//}

/// <summary>
/// D3D11のネイティブポインターを取得します。
/// </summary>
/// <returns></returns>
ID3D11VertexShader* BasicVertexShader::GetNativePointer()
{
	return shader.Get();
}

/// <summary>
/// このシェーダーのバイトコードを取得します。
/// </summary>
/// <returns></returns>
const BYTE* BasicVertexShader::GetBytecode()
{
	return g_BasicVertexShader;
}

/// <summary>
/// バイトコードのサイズを取得します。
/// </summary>
/// <returns></returns>
SIZE_T BasicVertexShader::GetBytecodeLength()
{
	return ARRAYSIZE(g_BasicVertexShader);
}

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphics"></param>
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

//// デストラクタ―
///// <summary>
///// 
///// </summary>
//BasicGeometryShader::~BasicGeometryShader()
//{
//
//}

/// <summary>
/// D3D11のネイティブポインターを取得します。
/// </summary>
/// <returns></returns>
ID3D11GeometryShader* BasicGeometryShader::GetNativePointer()
{
	return shader.Get();
}

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphics"></param>
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

//// デストラクタ―
///// <summary>
///// 
///// </summary>
//BasicPixelShader::~BasicPixelShader()
//{
//
//}

/// <summary>
/// D3D11のネイティブポインターを取得します。
/// </summary>
/// <returns></returns>
ID3D11PixelShader* BasicPixelShader::GetNativePointer()
{
	return shader.Get();
}