#pragma once

#include <Windows.h>
#include <comdef.h>
#include <system_error>
#include <source_location>
#include <string>

namespace GameLibrary {

	/// <summary>
	/// 直前のシステムエラー メッセージをデバッグ出力に表示します。
	/// </summary>
	/// <return></return>
	inline void OutputLastError(const std::source_location& location = std::source_location::current()) noexcept
	{
		const auto errorCode = GetLastError();

		char message[512] = {};
		sprintf_s(
			message, std::size(message),
			"%s(%d,%d): error %d",
			location.file_name(), location.line(), location.column(), errorCode);
		OutputDebugStringA(message);
		OutputDebugStringA(": ");

		LPWSTR buffer = nullptr;
		FormatMessageW(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, LANG_USER_DEFAULT,
			reinterpret_cast<LPWSTR>(&buffer), 0, NULL);
		OutputDebugStringW(buffer);
		LocalFree(buffer);
	}

	/// <summary>
	/// 直前のシステムエラーを例外としてスローします。
	/// </summary>
	inline void ThrowLastError(const std::source_location& location = std::source_location::current())
	{
		const auto errorCode = GetLastError();

		char message[512] = {};
		sprintf_s(
			message, std::size(message),
			"%s(%d,%d): error %d",
			location.file_name(), location.line(), location.column(), errorCode);
		OutputDebugStringA(message);
		OutputDebugStringA(": ");

		LPSTR buffer = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, LANG_USER_DEFAULT,
			reinterpret_cast<LPSTR>(&buffer), 0, NULL);
		OutputDebugStringA(buffer);
		LocalFree(buffer);

		throw std::system_error(
			std::error_code(errorCode, std::system_category()),
			message);
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
}
