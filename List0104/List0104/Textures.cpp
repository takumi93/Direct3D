//=============================================================================
// Textures.cpp
// 
//=============================================================================
#include "Game.h"
#include <comdef.h>
#include <new>

using namespace Microsoft::WRL;

// このクラスの新しいインスタンスを作成します。
Texture2D::Texture2D(
	ID3D11Device* graphicsDevice,
	UINT width, UINT height, DXGI_FORMAT format, bool mipChain)
{
	// テクスチャーを作成
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = (mipChain ? 0 : 1);
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		const auto hr = graphicsDevice->CreateTexture2D(&desc, nullptr, &texture);
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
	}

	// サンプラーステートを作成
	{
		D3D11_SAMPLER_DESC desc = {};
		//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		const auto hr = graphicsDevice->CreateSamplerState(&desc, &samplerState);
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
	}

	// テクスチャー用のシェーダーリソースビューを作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = (mipChain ? 0 : 1);

		const auto hr = graphicsDevice->CreateShaderResourceView(texture.Get(), &desc, &shaderResourceView);
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
	}
}

//// リソースを解放します。
//Texture2D::~Texture2D()
//{
//
//}

// テクスチャーのピクセルを変更します。
void Texture2D::SetData(const void* data)
{
	ComPtr<ID3D11Device> device;
	texture->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> context;
	device->GetImmediateContext(&context);
	context->UpdateSubresource(texture.Get(), 0, nullptr, data, 4 * sizeof(uint32_t), 0);
}

// D3D11のネイティブポインターを取得します。
ID3D11Texture2D* Texture2D::GetNativePointer()
{
	return texture.Get();
}

ID3D11SamplerState* Texture2D::GetSamplerState()
{
	return samplerState.Get();
}

ID3D11ShaderResourceView* Texture2D::GetShaderResourceView()
{
	return shaderResourceView.Get();
}