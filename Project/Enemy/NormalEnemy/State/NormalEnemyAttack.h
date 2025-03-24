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
	NormalEnemyAttack();

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
	~NormalEnemyAttack();
	

private:
	//0から始める
	const uint32_t RESTART_TIME = 0u;
	//増える値
	const uint32_t TIME_INCREASE_VALUE = 1u;
	//攻撃する時間
	const uint32_t JUST_ATTACK_TIME = 60u;
	//戻る時間
	const uint32_t RETURN_TIME = JUST_ATTACK_TIME * 2u;
private:
	//攻撃の時間
	uint32_t attackTime_ = 0;


};

