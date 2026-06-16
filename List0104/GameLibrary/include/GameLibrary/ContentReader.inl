#pragma once

#include <system_error>

namespace GameLibrary
{
	/// <summary>
	/// バイナリの読み込み処理
	/// </summary>
	/// <typeparam name="T">intやfloat等の変数</typeparam>
	/// <returns>読み込んだ変数のbyteを出力</returns>
	template<typename T>
	inline T ContentReader::Read()
	{
		auto result = T{};
		// resultを無理やりcharに変換してreadで読ませている
		if (!stream.read(reinterpret_cast<char*>(&result), sizeof result)) {
			throw std::system_error(errno, std::generic_category());
		}
		return result;
	}

	/// <summary>
	/// 固定長文字列を読むための関数
	/// </summary>
	/// <typeparam name="_Size"></typeparam>
	/// <param name="output"></param>
	template<size_t _Size>
	inline void ContentReader::ReadChars(char(&output)[_Size])
	{
		if (!stream.read(output, sizeof output)) {
			throw std::system_error(errno, std::generic_category());
		}
	}
}
