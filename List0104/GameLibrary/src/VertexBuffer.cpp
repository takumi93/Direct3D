#include <GameLibrary/Buffer.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

VertexBuffer::VertexBuffer(ID3D11Device5* graphicsDevice, UINT stride, UINT count)
	: GraphicsResource(graphicsDevice), stride(stride), count(count)
{

}

/// <summary>
/// 頂点情報一つ分のサイズを取得します。
/// </summary>
/// <returns>頂点の幅(バイト数)</returns>
UINT VertexBuffer::GetStride() const noexcept
{
	return stride;
}

/// <summary>
/// 頂点情報の数を取得します。
/// </summary>
/// <returns>頂点情報の数</returns>
UINT VertexBuffer::GetCount() const noexcept
{
	return count;
}

/// <summary>
/// Direct3D 11 ネイティブのポインターを取得します。
/// </summary>
/// <returns>ID3D11Buffer ポインター</returns>
ID3D11Buffer* VertexBuffer::GetNativePointer() noexcept
{
	return buffer.Get();
}

StaticVertexBuffer::StaticVertexBuffer(ID3D11Device5* graphicsDevice, UINT stride, UINT count, const void* source)
	: VertexBuffer(graphicsDevice, stride, count)
{
	const auto desc = D3D11_BUFFER_DESC{
		.ByteWidth = stride * count,
		.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	const auto initialData = D3D11_SUBRESOURCE_DATA{
		.pSysMem = source,
		.SysMemPitch = 0,
		.SysMemSlicePitch = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateBuffer(&desc, &initialData, &buffer));
}

StaticVertexBuffer::StaticVertexBuffer(ID3D11Device5* graphicsDevice, UINT stride, UINT count)
	: VertexBuffer(graphicsDevice, stride, count)
{
	const auto desc = D3D11_BUFFER_DESC{
		.ByteWidth = stride * count,
		.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateBuffer(&desc, NULL, &buffer));
}
