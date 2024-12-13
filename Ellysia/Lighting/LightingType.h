#pragma once

/**
 * @file LightingType.h
 * @brief ライティングの種類の列挙型
 * @author 茂木翼
 */

/// <summary>
/// ライティングの種類
/// </summary>
enum LightingType {
	//無し
	None,
	//平行光源
	Directional,
	//点光源
	Point,
	//スポット
	Spot,
	//環境マップ
	EnviromentMap,

};