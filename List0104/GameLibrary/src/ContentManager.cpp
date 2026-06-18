#include <GameLibrary/ContentManager.h>
#include <GameLibrary/Utility.h>
#include <DirectXTex.h>
#include <d3d11_4.h>
#include <memory>
#include <string>
#include <fstream>
#include <format>
#include <vector>
#include <system_error>

using namespace GameLibrary;
using namespace DirectX;

ContentManager::ContentManager(ID3D11Device5* graphicsDevice) noexcept
	: GraphicsResource(graphicsDevice)
{

}

/// <summary>
/// コンテンツを読み込みます。
/// </summary>
/// <param name="assetName">ファイル名</param>
/// <returns>読み込んだオブジェクト</returns>
std::shared_ptr<Texture2D> ContentManager::LoadTexture2D(const std::wstring& assetName)
{
	auto input = std::ifstream{};

	try {
		// ファイルを開く
		input.open(assetName, std::ios::binary);
		if (!input) {
			auto message = std::format("ファイルを開けませんでした: {}\n", ToString(assetName));
			throw std::system_error(errno, std::generic_category(), message);
		}
		input.seekg(0, std::ios::end);
		const auto end = input.tellg();
		if (end < 0) {
			auto message = std::format("ファイル サイズを取得できませんでした: {}\n", ToString(assetName));
			throw std::system_error(errno, std::generic_category(), message);
		}
		const auto fileSize = static_cast<size_t>(end);
		input.seekg(0, std::ios::beg);

		auto reader = std::make_unique<ContentReader>(*this, input, assetName);

		// ファイルの全データを読み込む
		auto source = reader->ReadBytes(fileSize);

		// 画像ファイルを読み込む
		auto metaData = TexMetadata{};
		auto image = ScratchImage{};
		ThrowIfFailed(LoadFromWICMemory(source.data(), fileSize, WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image));

		// 完全なミップマップを作成
		auto mipmaps = ScratchImage{};
		ThrowIfFailed(GenerateMipMaps(
			image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, 0,
			mipmaps));
		image.Release();
		image = std::move(mipmaps);
		metaData = image.GetMetadata();

		// BC7圧縮
		auto ddsImage = ScratchImage{};
		ThrowIfFailed(Compress(image.GetImages(), image.GetImageCount(), metaData,
			DXGI_FORMAT::DXGI_FORMAT_BC7_UNORM_SRGB,
			TEX_COMPRESS_FLAGS::TEX_COMPRESS_DEFAULT, TEX_THRESHOLD_DEFAULT,
			ddsImage));
		image.Release();
		image = std::move(ddsImage);
		metaData = image.GetMetadata();

		// Texture2Dを作成
		auto initialData = std::vector<D3D11_SUBRESOURCE_DATA>{ metaData.mipLevels };
		for (size_t mipLevel = 0; mipLevel < metaData.mipLevels; mipLevel++) {
			initialData[mipLevel].pSysMem = image.GetImage(mipLevel, 0, 0)->pixels;
			initialData[mipLevel].SysMemPitch = static_cast<UINT>(image.GetImage(mipLevel, 0, 0)->rowPitch);
			initialData[mipLevel].SysMemSlicePitch = static_cast<UINT>(image.GetImage(mipLevel, 0, 0)->slicePitch);
		}
		const auto result = std::make_shared<Texture2D>(GetDevice(),
			static_cast<UINT>(metaData.width), static_cast<UINT>(metaData.height), static_cast<UINT>(metaData.mipLevels), metaData.format,
			initialData);

		input.close();
		return result;
	}
	catch (const std::exception& error) {
		input.close();
		throw error;
	}
}
