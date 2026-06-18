#pragma once

#include <Windows.h>
#include <system_error>
#include <source_location>
#include <string>
#include <format>
#include <comdef.h>

namespace GameLibrary
{
	/// <summary>
	/// 直前のシステムエラー メッセージをデバッグ出力に表示します。
	/// </summary>
	/// <return>エラー メッセージ</return>
	inline std::string OutputLastError(DWORD lastErrorCode = GetLastError(), const std::source_location& location = std::source_location::current()) noexcept
	{
		LPSTR buffer = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lastErrorCode, LANG_USER_DEFAULT,
			reinterpret_cast<LPSTR>(&buffer), 0, NULL);
		const auto message = std::format(
			"{}({},{}): error {}: {}",
			location.file_name(), location.line(), location.column(), lastErrorCode, buffer);
		LocalFree(buffer);

		OutputDebugStringA(message.c_str());

		return message;
	}

	/// <summary>
	/// 直前のシステムエラーから作成された例外を表します。
	/// </summary>
	class LastError : public std::system_error
	{
	public:
		LastError(DWORD lastErrorCode = GetLastError(), const std::source_location& location = std::source_location::current())
			:std::system_error(lastErrorCode, std::system_category(), OutputLastError(lastErrorCode, location)) {}
	};

	/// <summary>
	/// 直前のシステムエラーを例外としてスローします。
	/// </summary>
	inline void ThrowLastError(DWORD lastErrorCode = GetLastError(), const std::source_location& location = std::source_location::current())
	{
		throw LastError(lastErrorCode, location);
	}

	/// <summary>
	/// 直前のシステムエラーを例外としてスローします。
	/// </summary>
	inline void ThrowIfFailed(HRESULT hr, const std::source_location& location = std::source_location::current())
	{
		if (FAILED(hr)) {
			const auto error = _com_error(hr);

			char message[512] = {};
			sprintf_s(
				message, std::size(message),
				"%s(%d,%d): error 0x%X",
				location.file_name(), location.line(), location.column(), hr);
			OutputDebugStringA(message);
			OutputDebugStringA(": ");

			OutputDebugString(error.ErrorMessage());
			OutputDebugString(TEXT("\n"));

			throw error;
		}
	}

	/// <summary>
	/// 指定した文字列を std::string 型の文字列に変換します。
	/// </summary>
	/// <param name="value">変換する文字列</param>
	/// <param name="codePage">
	/// 変換の実行に使用するコード ページ。CP_ACP を指定した場合はマルチバイト文字列に、
	/// CP_UTF8を指定した場合は UTF-8文字列に変換します。
	/// </param>
	/// <returns> std::string 型の文字列</returns>
	inline std::string ToString(const std::wstring& value, UINT codePage = CP_ACP)
	{
		auto size = WideCharToMultiByte(codePage, 0,
			value.c_str(), static_cast<int>(value.size()),
			nullptr, 0, nullptr, nullptr);
		if (size == 0) {
			throw LastError();
		}
		std::string result(size, 0);
		size = WideCharToMultiByte(codePage, 0,
			value.c_str(), static_cast<int>(value.size()),
			result.data(), size, nullptr, nullptr);
		if (size == 0) {
			throw LastError();
		}
		return result;
	}

	/// <summary>
	/// マルチ バイト文字列をワイド文字列に変換します。
	/// </summary>
	/// <param name="value">変換元のマルチ バイト文字列</param>
	/// <returns>変換後のワイド文字列</returns>
	inline std::wstring ToWideString(const std::string& value)
	{
		auto size = MultiByteToWideChar(CP_ACP, 0,
			value.c_str(), static_cast<int>(value.size()),
			nullptr, 0);
		if (size == 0) {
			ThrowLastError();
		}
		std::wstring result(size, 0);
		size = MultiByteToWideChar(CP_ACP, 0,
			value.c_str(), static_cast<int>(value.size()),
			result.data(), size);
		if (size == 0) {
			ThrowLastError();
		}
		return result;
	}
}
