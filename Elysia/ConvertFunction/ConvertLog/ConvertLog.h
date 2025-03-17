#pragma once

/**
 * @file ConvertLog.h
 * @brief Logに関係する関数
 * @author 茂木翼
 */

#include <cstdint>
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>

/// <summary>
/// Logに関係する関数
/// </summary>
namespace ConvertString {
	/// <summary>
	///	ログの出力
	/// </summary>
	/// <param name="text"></param>
	void Log(const std::string& text);

	/// <summary>
	/// stringからwstringに変換
	/// </summary>
	/// <param name="text">文字列</param>
	/// <returns></returns>
	std::wstring ToWString(const std::string& text);

	/// <summary>
	/// wstringからstringに変換
	/// </summary>
	/// <param name="text">W文字列</param>
	/// <returns></returns>
	std::string ToString(const std::wstring& text);

}

