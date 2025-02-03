#pragma once

/**
 * @file Easing.h
 * @brief イージング関数のまとめ
 * @author 茂木翼
 */

//ここでのイージング関数はここから持ってきた
//https://easings.net/ja

#include <cmath>
#include <numbers>

#include "SingleCalculation.h"

/// <summary>
/// イージング
/// </summary>
namespace Easing{
	
	#pragma region Sine

	/// <summary>
	/// 早くなっていくイージング
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline float EaseInSine(const float& value) {
		//計算
		float result = 1.0f - std::cosf((value * std::numbers::pi_v<float>) / 2.0f);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// 遅くなっていくイージング
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline float EaseOutSine(const float& value) {
		//計算
		float result = std::sinf((value * std::numbers::pi_v<float>) / 2.0f);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// InとOutの両方を使ったイージング
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline float EaseInOutSine(const float& value) {
		//計算
		float result = -(std::cosf(std::numbers::pi_v<float> *value) - 1.0f) / 2.0f;
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	#pragma endregion


};
