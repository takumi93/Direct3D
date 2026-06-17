//=============================================================================
// Textures.cpp
// 
//=============================================================================
#include <GameLibrary/Texture2D.h>
#include <GameLibrary/Utility.h>
#include <memory>

using namespace GameLibrary;
using namespace Microsoft::WRL;

/// <summary>
/// このクラスの新しいインスタンスを作成します。
/// </summary>
/// <param name="graphicsDevice"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="format"></param>
/// <param name="source"></param>
Texture2D::Texture2D(ID3D11Device5* graphicsDevice,
	UINT width, UINT height, DXGI_FORMAT format, const std::byte* source)
	: GraphicsResource(graphicsDevice)
	, width(width), height(height), format(format)
{
	// テクスチャーを作成
	const auto textureDesc = D3D11_TEXTURE2D_DESC1{
		.Width = width,
		.Height = height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = format,
		.SampleDesc = {.Count = 1, .Quality = 0 },
		.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
	};

	const auto rowPitch = static_cast<UINT>(textureDesc.Width * sizeof(uint32_t));
	const auto initialData = D3D11_SUBRESOURCE_DATA{
		.pSysMem = source,
		.SysMemPitch = rowPitch,
		.SysMemSlicePitch = rowPitch * height,
	};
	ThrowIfFailed(graphicsDevice->CreateTexture2D1(&textureDesc, &initialData, &nativePointer));

	// サンプラーステートを作成
	const auto samplerDesc = D3D11_SAMPLER_DESC{
		.Filter = filter,
		.AddressU = addressU,
		.AddressV = addressV,
		.AddressW = addressW,
		.MipLODBias = mipLODBias,
		.MaxAnisotropy = maxAnisotropy,
		.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER,
		.BorderColor = { 0.0f, 0.0f, 0.0f, 0.0f },
		.MinLOD = 0,
		.MaxLOD = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateSamplerState(&samplerDesc, &samplerState));

	// テクスチャー用のシェーダーリソースビューを作成
	const auto viewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
		.Format = textureDesc.Format,
		.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {.MostDetailedMip = 0, .MipLevels = 1, .PlaneSlice = 0, },
	};
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView1(nativePointer.Get(), &viewDesc, &view));
}

/// <summary>
/// 現在の設定でサンプラー ステートを更新します。
/// </summary>
void Texture2D::Apply()
{
	// サンプラーステート
	const auto samplerDesc = D3D11_SAMPLER_DESC{
		.Filter = filter,
		.AddressU = addressU,
		.AddressV = addressV,
		.AddressW = addressW,
		.MipLODBias = mipLODBias,
		.MaxAnisotropy = maxAnisotropy,
		.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER,
		.BorderColor = { 0.0f, 0.0f, 0.0f, 0.0f },
		.MinLOD = 0,
		.MaxLOD = 0,
	};
	ThrowIfFailed(GetDevice()->CreateSamplerState(&samplerDesc, &samplerState));
}

/// <summary>
/// このクラスの新しいインスタンスを作成します。
/// </summary>
/// <param name="graphicsDevice"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="mipLevels"></param>
/// <param name="format"></param>
/// <param name="initialData"></param>
Texture2D::Texture2D(ID3D11Device5* graphicsDevice,
	UINT width, UINT height, UINT mipLevels, DXGI_FORMAT format,
	const std::vector<D3D11_SUBRESOURCE_DATA>& initialData)
	: GraphicsResource(graphicsDevice)
	, width(width), height(height), mipLevels(mipLevels), format(format)
{
	// テクスチャーを作成
	const auto textureDesc = D3D11_TEXTURE2D_DESC1{
		.Width = width,
		.Height = height,
		.MipLevels = mipLevels,
		.ArraySize = 1,
		.Format = format,
		.SampleDesc = {.Count = 1, .Quality = 0, },
		.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE ,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateTexture2D1(&textureDesc, initialData.data(), &nativePointer));

	// テクスチャー用のシェーダーリソースビューを作成
	const auto viewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC1{
		.Format = textureDesc.Format,
		.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {.MostDetailedMip = 0, .MipLevels = mipLevels, .PlaneSlice = 0, },
	};
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView1(nativePointer.Get(), &viewDesc, &view));

	// サンプラーステートを作成
	const auto samplerDesc = D3D11_SAMPLER_DESC{
		.Filter = filter,
		.AddressU = addressU,
		.AddressV = addressV,
		.AddressW = addressW,
		.MipLODBias = mipLODBias,
		.MaxAnisotropy = maxAnisotropy,
		.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER,
		.BorderColor = { 0.0f, 0.0f, 0.0f, 0.0f },
		.MinLOD = 0,
		.MaxLOD = 0,
	};
	ThrowIfFailed(graphicsDevice->CreateSamplerState(&samplerDesc, &samplerState));
}

///// <summary>
///// リソースを解放します。
///// </summary>
//Texture2D::~Texture2D()
//{
//
//}

///// <summary>
///// テクスチャーのピクセルを変更します。
///// </summary>
///// <param name="data"></param>
//void Texture2D::SetData(const void* data)
//{
//	ComPtr<ID3D11Device> device;
//	texture->GetDevice(&device);
//	ComPtr<ID3D11DeviceContext> context;
//	device->GetImmediateContext(&context);
//	context->UpdateSubresource(texture.Get(), 0, nullptr, data, 4 * sizeof(uint32_t), 0);
//}