//=============================================================================
// Buffers.cpp
// 頂点バッファー、インデックスバッファー、定数バッファー機能が含まれます。
//=============================================================================
#include <GameLibrary/Game.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;
using namespace Microsoft::WRL;

VertexBuffer::VertexBuffer(std::shared_ptr<Graphics> graphics, UINT byteWidth)
{
	// 頂点バッファーを作成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;	// 作成するバッファーのサイズ(bytes) x:4bytes,y:4bytes,z:4bytes total 12bytes
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;					// バッファーの使用方法
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// Vertex Bufferとして利用する
	bufferDesc.CPUAccessFlags = 0;							// CPUからの読み書きに使わない場合は0
	bufferDesc.MiscFlags = 0;								// オプションのフラグ
	bufferDesc.StructureByteStride = 0;						// 頂点バッファーとして使うなら0
	ThrowIfFailed(graphics->GetDevice()->CreateBuffer(&bufferDesc, NULL, &buffer));

	this->graphics = graphics;
	this->buffer = buffer;
}

///// <summary>
///// VerTexBufferのリソースを解放します。
///// </summary>
//VertexBuffer::~VertexBuffer()
//{
//
//}

// D3D11のネイティブポインターを取得します。
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// バッファーにデータを設定します。
void VertexBuffer::SetData(const void* data)
{
	// バッファーにdataを転送(コピー)する
	graphics->GetDeviceContext()->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

//void VertexBuffer::Bind(UINT slot) {
//	UINT offset = 0;
//	ID3D11Buffer* buf = buffer.Get();
//
//	auto ctx = graphics->GetDeviceContext();
//
//	ctx->IASetVertexBuffers(
//		slot,
//		1,
//		&buf,
//		&stride,
//		&offset);
//}

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphics"></param>
/// <param name="byteWidth"></param>
IndexBuffer::IndexBuffer(std::shared_ptr<Graphics> graphics, UINT byteWidth)
{
	// 作成するバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth * sizeof(UINT32);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// バッファーを作成
	ThrowIfFailed(graphics->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &buffer));

	this->graphics = graphics;
	this->buffer = buffer;
}

///// <summary>
///// IndexBufferのリソースを解放します。
///// </summary>
//IndexBuffer::~IndexBuffer()
//{
//
//}

/// <summary>
/// D3D11のネイティブポインターを取得します。
/// </summary>
/// <returns></returns>
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer.Get();
}

/// <summary>
/// バッファーにデータを設定します。
/// </summary>
/// <param name="data"></param>
void IndexBuffer::SetData(const void* data)
{
	// バッファーにdataを転送(コピー)する
	graphics->GetDeviceContext()->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

/// <summary>
/// ConstantBufferクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphics"></param>
/// <param name="byteWidth"></param>
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
	ThrowIfFailed(graphics->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &buffer));

	this->graphics = graphics;
	this->buffer = buffer;
}

///// <summary>
///// ConstantBufferのリソースを解放します。
///// </summary>
//ConstantBuffer::~ConstantBuffer()
//{
//
//}

/// <summary>
/// D3D11のネイティブポインターを取得します。
/// </summary>
/// <returns></returns>
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer.Get();
}

/// <summary>
/// バッファーにデータを設定します。
/// </summary>
/// <param name="data"></param>
void ConstantBuffer::SetData(const void* data)
{
	// バッファーにdataを転送(コピー)する
	graphics->GetDeviceContext()->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

