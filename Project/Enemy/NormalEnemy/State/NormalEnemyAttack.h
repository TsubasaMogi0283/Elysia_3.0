#pragma once


/**
 * @file NormalEnemyAttack.h
 * @brief 通常の敵が攻撃するクラス
 * @author 茂木翼
 */

#include "BaseNormalEnemyState.h"


/// <summary>
/// 通常の敵が攻撃する
/// </summary>
class NormalEnemyAttack : public BaseNormalEnemyState {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	NormalEnemyAttack() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="normalEnemy">通常の敵</param>
	void Update(NormalEnemy* normalEnemy)override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalEnemyAttack() = default;
	
private:
	//攻撃の時間
	int32_t attackTime_ = 0;


};

