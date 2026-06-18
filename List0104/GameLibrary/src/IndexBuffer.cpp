#include <GameLibrary/Buffer.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

namespace
{
	inline constexpr IndexFormat DxgiToIndexFormat(DXGI_FORMAT format)
	{
		return (format == DXGI_FORMAT::DXGI_FORMAT_R16_UINT) ? IndexFormat::UInt16 : IndexFormat::UInt32;
	}

	inline constexpr DXGI_FORMAT IndexToDxgiFormat(IndexFormat format)
	{
		return (format == IndexFormat::UInt16) ? DXGI_FORMAT::DXGI_FORMAT_R16_UINT : DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}
}

IndexBuffer::IndexBuffer(ID3D11Device5* graphicsDevice, IndexFormat format, UINT indexCount, const void* source)
	: GraphicsResource(graphicsDevice), format(IndexToDxgiFormat(format)), count(indexCount)
{
	const auto desc = D3D11_BUFFER_DESC{
		.ByteWidth = static_cast<UINT>(format == IndexFormat::UInt16 ? sizeof(uint16_t) : sizeof(uint32_t)) * indexCount,
		.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER,
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

/// <summary>
/// このインデックス バッファーの DXGI_FORMAT を取得します。
/// </summary>
/// <returns>DXGI_FORMAT フォーマット</returns>
DXGI_FORMAT IndexBuffer::GetFormat() const noexcept
{
	return format;
}

/// <summary>
/// インデックスの数を取得します。
/// </summary>
/// <returns>インデックスの数</returns>
UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}

/// <summary>
/// Direct3D 11 ネイティブのポインターを取得します。
/// </summary>
/// <returns>ID3D11Buffer ポインター</returns>
ID3D11Buffer* IndexBuffer::GetNativePointer() noexcept
{
	return buffer.Get();
}
