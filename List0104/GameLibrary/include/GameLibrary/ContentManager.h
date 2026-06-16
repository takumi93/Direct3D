#pragma once

#include <GameLibrary/Graphics.h>
#include <GameLibrary/Texture2D.h>
#include <d3d11_4.h>
#include <memory>
#include <string>
#include <istream>
#include <vector>

namespace GameLibrary
{
	/// <summary>
	/// コンテンツを読み込む機能を提供します。
	/// </summary>
	class ContentManager final : public GraphicsResource
	{
	public:
		ContentManager(ID3D11Device5* graphicsDevice) noexcept;
		~ContentManager() = default;

		std::shared_ptr<Texture2D> LoadTexture2D(const std::wstring& assetName);
	};

	/// <summary>
	/// ContentManager.Loadの大部分を実装するワーカーオブジェクトを定義します。
	/// </summary>
	class ContentReader final
	{
	public:
		ContentReader(ContentManager& contentManager, std::istream& stream, const std::wstring& assetName) noexcept;
		~ContentReader() = default;

		template<typename T>
		inline T Read();

		template<size_t _Size>
		inline void ReadChars(char(&output)[_Size]);

		inline auto ReadByte() { return Read<std::byte>(); }
		inline auto ReadChar() { return Read<char>(); }
		inline auto ReadUInt8() { return Read<uint8_t>(); }
		inline auto ReadUInt16() { return Read<uint16_t>(); }
		inline auto ReadUInt32() { return Read<uint32_t>(); }
		inline auto ReadUInt64() { return Read<uint64_t>(); }

		int32_t Read7BitEncodedInt();
		int64_t Read7BitEncodedInt64();

		std::vector<std::byte> ReadBytes(size_t count);

		ID3D11Device5* GetGraphicsDevice() noexcept;

	private:
		ContentManager& contentManager;
		std::istream& stream;
		std::wstring assetName;
	};
}

#include <GameLibrary/ContentReader.inl>
