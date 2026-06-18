#include <GameLibrary/ContentManager.h>
#include <GameLibrary/Utility.h>

using namespace GameLibrary;

ContentReader::ContentReader(ContentManager& contentManager, std::istream& stream, const std::wstring& assetName) noexcept
	: contentManager(contentManager), stream(stream), assetName(assetName)
{

}

int32_t ContentReader::Read7BitEncodedInt()
{
	auto result = int32_t{ 0 };
	auto lShift = size_t{ 0 };
	// 32bitsの整数値をエンコーディングした場合は、最大5バイト
	for (size_t index = 0; index < 5; index++) {
		auto value = static_cast<int>(ReadByte());
		result |= static_cast<int32_t>(value & 0x7f) << lShift;
		if ((value & 0x80) == 0) {
			return result;
		}

		lShift += 7;
	}
	// データが壊れている場合
	throw std::system_error(std::make_error_code(std::io_errc::stream));
}

int64_t ContentReader::Read7BitEncodedInt64()
{
	auto result = int64_t{ 0 };
	auto lShift = size_t{ 0 };
	// 64bitsの整数値をエンコーディングした場合は、最大10バイト
	for (size_t index = 0; index < 10; index++) {
		auto value = static_cast<int>(ReadByte());
		result |= static_cast<int64_t>(value & 0x7f) << lShift;
		if ((value & 0x80) == 0) {
			return result;
		}

		lShift += 7;
	}
	// データが壊れている場合
	throw std::system_error(std::make_error_code(std::io_errc::stream));
}

std::vector<std::byte> ContentReader::ReadBytes(size_t count)
{
	std::vector<std::byte> result{ count };
	if (!stream.read(reinterpret_cast<char*>(result.data()), count)) {
		throw std::system_error(errno, std::generic_category());
	}
	return result;
}

ID3D11Device5* ContentReader::GetGraphicsDevice() noexcept
{
	return contentManager.GetDevice();
}
