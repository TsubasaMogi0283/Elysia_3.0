#pragma once

/**
 * @file ConvertColor.h
 * @brief 色変換
 * @author 茂木翼
 */

#include <cstdint>

#include "Vector4.h"


// 白
const uint32_t WHITE =0xFFFFFFFF;
// 黒
const uint32_t BLACK =0x000000FF;
// 赤
const uint32_t RED =0xFF0000FF;
// 緑
const uint32_t GREEN =0x00FF0FF;
// 青
const uint32_t BLUE = 0x0000FFFF;
//黄色
const uint32_t YELLOW = 0xFFFF00FF;
// 紫
const uint32_t PURPLE = 0xFF00FFFF;

/// <summary>
/// 色変換 
/// </summary>
/// <param name="color"></param>
/// <returns></returns>
Vector4 ColorAdapter(const uint32_t& color);