#pragma once

/**
 * @file Easing.h
 * @brief イージング関数のまとめ
 * @author 茂木翼
 */

//ここでのイージング関数はここから持ってきた
//https://easings.net/ja

template <typename T>


/// <summary>
/// イージング
/// </summary>
namespace Easing{
	

	/// <summary>
	/// Sineを使ったイージング
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	T EaseInSine(const T& value);
};

