#include "ConvertLog.h"


void ConvertString::Log(const std::string& text) {
	OutputDebugStringA(text.c_str());
}


std::wstring ConvertString::ToWString(const std::string& text) {
	//何もなかったら空を返す
	if (text.empty()){
		return std::wstring();
	}

	//マルチバイトに変換
	int32_t sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&text[0]), static_cast<int>(text.size()), NULL, 0);
	if (sizeNeeded == 0){
		return std::wstring();
	}

	//W文字列に変換
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&text[0]), static_cast<int>(text.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString::ToString(const std::wstring& text) {
	//何もなかったら空を返す
	if (text.empty()){
		return std::string();
	}

	//マルチバイトに変換
	int32_t sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0){
		return std::string();
	}

	//通常の文字列に変換
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}