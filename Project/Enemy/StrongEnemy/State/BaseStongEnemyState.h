#pragma once

/**
 * @file BaseStongEnemyState.h
 * @brief 強敵の行動パターンの基底クラス
 * @author 茂木翼
 */

#include <memory>
#include <string>

#include "Vector3.h"


/// <summary>
/// 強敵
/// </summary>
class StrongEnemy;


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
	virtual void Update(StrongEnemy* strongEnemy) = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseStongEnemyState() = default;

public:

	//変更した後の値も使いたいので参照渡しにしている
		

	/// <summary>
	/// 方向の設定
	/// </summary>
	/// <param name="direction"></param>
	inline void SetDirection(Vector3& direction) {
		this->direction_ = direction;
	}

	/// <summary>
	/// 方向を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDirection()const {
		return direction_;
	}

	/// <summary>
	/// スピードの設定
	/// </summary>
	/// <param name="speed"></param>
	inline void SetSpeed(Vector3& speed) {
		this->speed_ = speed;
	}

	/// <summary>
	/// 状態の名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetStateName()const {
		return stateName_;
	}

protected:
	//状態の名前
	std::string stateName_ = "";

	//方向
	Vector3 direction_ ={};
	//スピード
	Vector3 speed_ = {};
};