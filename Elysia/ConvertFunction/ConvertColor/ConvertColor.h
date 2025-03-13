#pragma once

/**
 * @file ConvertColor.h
 * @brief 色変換
 * @author 茂木翼
 */


#include "Vector4.h"

//白
#define WHITE = 0xFFFFFFFF;
// 黒
#define BLACK = 0x000000FF;
// 赤
#define RED = 0xFF0000FF;
// 緑
#define GREEN = 0x00FF00FF;
// 青
#define BLUE = 0x0000FFFF;
//黄色
#define YELLOW = 0xFFFF00FF;
// 紫
#define PURPLE = 0xFF00FFFF;


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