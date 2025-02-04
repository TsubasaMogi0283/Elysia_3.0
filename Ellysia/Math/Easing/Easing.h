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
	/// SineのIn
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseInSine(const float& value) {
		//計算
		float result = 1.0f - std::cosf((value * std::numbers::pi_v<float>) / 2.0f);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// SineのOut
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseOutSine(const float& value) {
		//計算
		float result = std::sinf((value * std::numbers::pi_v<float>) / 2.0f);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// SineのInとOut
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseInOutSine(const float& value) {
		//計算
		float result = -(std::cosf(std::numbers::pi_v<float> *value) - 1.0f) / 2.0f;
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	#pragma endregion

	#pragma region Quad
	/// <summary>
	///　QuadのIn
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseInQuad(const float& value) {
		//計算
		float result = value * value;
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// QuadのOut
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseOutQuad(const float& value) {
		//計算
		float result = 1.0f - (1.0f - value) * (1.0f - value);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// QuadのInとOut
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline float EaseInOutQuad(const float& value) {
		//計算
		float result = 0.0f;
		if (value < 0.5f) {
			result = 2.0f * value * value;
		}
		else {
			result = 1.0f - std::pow(-2.0f * value + 2.0f, 2.0f) / 2.0f;
		}

		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	}

	#pragma endregion

	#pragma region Cubic
	/// <summary>
	///　CubicのIn
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseInCubic(const float& value) {
		//計算
		float result = value * value*value;
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// CubicのOut
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseOutCubic(const float& value) {
		//計算
		float result = 1.0f - std::powf(1.0f - value, 3.0f);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// CubicのInとOut
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline float EaseInOutCubic(const float& value) {
		//計算
		float result = 0.0f;
		if (value < 0.5f) {
			result = 4.0f * value * value * value;
		}
		else {
			result = 1.0f - std::powf(-2.0f * value + 2.0f, 3.0f) / 2.0f;
		}

		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	}
	#pragma endregion


	#pragma region Quart
	/// <summary>
	/// QuartのIn
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseInQuart(const float& value) {
		//計算
		float result = value * value * value * value;
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// QuartのOut
	/// </summary>
	/// <param name="value">値(t)</param>
	/// <returns></returns>
	inline float EaseOutQuart(const float& value) {
		//計算
		float result = 1.0f - std::powf(1.0f - value, 4.0f);
		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	};

	/// <summary>
	/// QuartのInとOut
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	inline float EaseInOutQuart(const float& value) {
		//計算
		float result = 0.0f;
		if (value < 0.5f) {
			result = 8.0f * value * value * value * value;
		}
		else {
			result = 1.0f - std::powf(-2.0f * value + 2.0f, 4.0f) / 2.0f;
		}

		//0.0fから1.0fに変換する
		return SingleCalculation::Clamp(0.0f, 1.0f, result);
	}
	#pragma endregion

};
