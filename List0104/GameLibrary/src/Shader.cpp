#include <GameLibrary/Shader.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

VertexShader::VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: BaseShader(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreateVertexShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

void VertexShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->VSSetShader(shader.Get(), NULL, 0);
}

GeometryShader::GeometryShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: BaseShader(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreateGeometryShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

void GeometryShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->GSSetShader(shader.Get(), NULL, 0);
}

PixelShader::PixelShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength)
	: BaseShader(graphicsDevice)
{
	ThrowIfFailed(graphicsDevice->CreatePixelShader(
		shaderBytecode, bytecodeLength, NULL, &shader));
}

void PixelShader::Apply(ID3D11DeviceContext4* deviceContext) noexcept
{
	deviceContext->PSSetShader(shader.Get(), NULL, 0);
}