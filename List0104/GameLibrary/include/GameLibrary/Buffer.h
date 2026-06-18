#pragma once

#include <GameLibrary/Graphics.h>
#include <wrl/client.h>
#include <d3d11_4.h>

namespace GameLibrary
{
	// 頂点バッファーを表します。
	class VertexBuffer : public GraphicsResource
	{
	public:
		virtual ~VertexBuffer() = default;

		UINT GetStride() const noexcept;
		UINT GetCount() const noexcept;
		ID3D11Buffer* GetNativePointer() noexcept;

	protected:
		// このクラスの新しいインスタンスを初期化します。
		VertexBuffer(ID3D11Device5* graphicsDevice, UINT stride, UINT count);

		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT stride = 0;
		UINT count = 0;
	};

	class StaticVertexBuffer final : public VertexBuffer
	{
	public:
		StaticVertexBuffer(ID3D11Device5* graphicsDevice, UINT stride, UINT count, const void* source);
		StaticVertexBuffer(ID3D11Device5* graphicsDevice, UINT stride, UINT count);
	};

	enum class IndexFormat
	{
		UInt16,
		UInt32,
	};

	// インデックスバッファーを表します。
	class IndexBuffer final : public GraphicsResource
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		IndexBuffer(ID3D11Device5* graphicsDevice, IndexFormat format, UINT indexCount, const void* source);

		DXGI_FORMAT GetFormat() const noexcept;
		UINT GetCount() const noexcept;
		ID3D11Buffer* GetNativePointer() noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		UINT count = 0;
	};

	// 定数バッファーを表します。
	class ConstantBuffer final : public GraphicsResource
	{
	public:
		// このクラスの新しいインスタンスを初期化します。
		ConstantBuffer(ID3D11Device5* graphicsDevice, UINT byteWidth);

		UINT GetByteWidth() const noexcept;
		void UpdateSubresource(const void* source) noexcept;

		ID3D11Buffer* GetNativePointer() noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT byteWidth = 0;
	};
}