#pragma once

/**
 * @file BaseStongEnemyState.h
 * @brief 強敵の行動パターンの基底クラス
 * @author 茂木翼
 */

#include "Vector3.h"


/// <summary>
/// 強敵の行動パターン
/// </summary>
class BaseStongEnemyState {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


protected:
	//方向
	Vector3 direction_ ={};
	//スピード
	Vector3 speed_ = {};
};