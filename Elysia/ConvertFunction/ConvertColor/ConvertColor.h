#pragma once

/**
 * @file ConvertColor.h
 * @brief 色変換
 * @author 茂木翼
 */


#include "Vector4.h"

/// <summary>
/// 色
/// </summary>
namespace Color {
	/// <summary>
	/// 色変換 
	/// </summary>
	/// <param name="color">色</param>
	/// <returns>色(Vector4)</returns>
	Vector4 ColorAdapter(const uint32_t& color);

}