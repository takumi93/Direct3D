#include <GameLibrary/Buffer.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;
using Microsoft::WRL::ComPtr;

ConstantBuffer::ConstantBuffer(ID3D11Device5* graphicsDevice, UINT byteWidth) 
	: GraphicsResource(graphicsDevice)
	, byteWidth(byteWidth)
{
	// 作成するバッファーについての記述
	D3D11_BUFFER_DESC desc = {
		.ByteWidth = static_cast<UINT>(byteWidth),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0
	};
	ThrowIfFailed(graphicsDevice->CreateBuffer(&desc, nullptr, &buffer));
}

/// <summary>
/// 定数バッファーのサイズを取得します。
/// </summary>
/// <returns>バッファーのサイズ(バイト単位)</returns>
UINT ConstantBuffer::GetByteWidth() const noexcept
{
	return byteWidth;
}

/// <summary>
/// 指定したデータで定数バッファーを更新します。
/// </summary>
/// <param name="source">更新元データ</param>
void ConstantBuffer::UpdateSubresource(const void* source) noexcept
{
	ComPtr<ID3D11DeviceContext3> context;
	GetDevice()->GetImmediateContext3(&context);
	context->UpdateSubresource1(buffer.Get(), 0, NULL, source, 0, 0, 0);
}

/// <summary>
/// Direct3D 11 ネイティブのポインターを取得します。
/// </summary>
/// <returns>ID3D11Buffer ポインター</returns>
ID3D11Buffer* ConstantBuffer::GetNativePointer() noexcept
{
	return buffer.Get();
}