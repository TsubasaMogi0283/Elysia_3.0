#include "ConvertLog.h"


//std::string ConvertString::ToString(const std::wstring& text) {
//	//何もなかったら空を返す
//	if (text.empty()){
//		return std::string();
//	}
//
//	//マルチバイトに変換
//	int32_t sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), NULL, 0, NULL, NULL);
//	if (sizeNeeded == 0){
//		return std::string();
//	}
//
//	//通常の文字列に変換
//	std::string result(sizeNeeded, 0);
//	WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), sizeNeeded, NULL, NULL);
//	return result;
//}