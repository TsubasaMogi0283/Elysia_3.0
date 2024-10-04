#pragma once
#include <cstdint>
#include <string>
#include <format>



#include <dxgidebug.h>
#include <dxcapi.h>

namespace ConvertString {
	/// <summary>
	///	ログの出力
	/// </summary>
	/// <param name="message"></param>
	void Log(const std::string& message);

	//string->wstring
	std::wstring ToWString(const std::string& str);

	//wstring->string
	std::string ToString(const std::wstring& str);

}

