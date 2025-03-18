#pragma once
#include <cstdint>
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>

#include "Vector4.h"

/// <summary>
/// 変換
/// </summary>
namespace Convert {
	
	/// <summary>
	/// テキスト
	/// </summary>
	namespace Text {
		/// <summary>
		/// ログ
		/// </summary>
		/// <param name="text"></param>
		inline void Log(const std::string& text) {
			OutputDebugStringA(text.c_str());
		}

	}

	/// <summary>
	/// 色
	/// </summary>
	namespace Color {

		/// <summary>
		/// 変換(uint32_t→Vector4) 
		/// </summary>
		/// <param name="color">色</param>
		/// <returns>色(Vector4)</returns>
		Vector4 ColorAdapter(const uint32_t& color);

	}
	

}