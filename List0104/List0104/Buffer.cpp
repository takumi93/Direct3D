//=============================================================================
// Buffers.cpp
// 頂点バッファー、インデックスバッファー、定数バッファー機能が含まれます。
//=============================================================================
#include "Game.h"
#include <comdef.h>

using namespace Microsoft::WRL;

VertexBuffer::VertexBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth)
{
	// 頂点バッファーを作成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(byteWidth);	// 作成するバッファーのサイズ(bytes) x:4bytes,y:4bytes,z:4bytes total 12bytes
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;					// バッファーの使用方法
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// Vertex Bufferとして利用する
	bufferDesc.CPUAccessFlags = 0;							// CPUからの読み書きに使わない場合は0
	bufferDesc.MiscFlags = 0;								// オプションのフラグ
	bufferDesc.StructureByteStride = 0;						// 頂点バッファーとして使うなら0
	const auto hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->graphics = graphics;
	this->buffer = buffer;
}

// バッファーにデータを設定します。
void VertexBuffer::SetData(const void* data)
{
	// バッファーにdataを転送(コピー)する
	graphics->GetDeviceContext()->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// D3D11のネイティブポインターを取得します。
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer.Get();
}

IndexBuffer::IndexBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth)
{
	// 作成するバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(byteWidth);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// バッファーを作成
	const auto hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &buffer);
	if (FAILED(hr) ) {
		throw _com_error(hr);
	}

	this->graphics = graphics;
	this->buffer = buffer;
}

void IndexBuffer::SetData(const void* data)
{
	// バッファーにdataを転送(コピー)する
	graphics->GetDeviceContext()->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// D3D11のネイティブポインターを取得します。
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer.Get();
}

ConstantBuffer::ConstantBuffer(std::shared_ptr<Graphics> graphics, size_t byteWidth)
{
	// 作成するバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(byteWidth);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	const auto hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &buffer);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	this->graphics = graphics;
	this->buffer = buffer;
}

void ConstantBuffer::SetData(const void* data)
{
	// バッファーにdataを転送(コピー)する
	graphics->GetDeviceContext()->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// D3D11のネイティブポインターを取得します。
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer.Get();
}