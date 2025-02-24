#pragma once

/**
 * @file StrongEnemyNoneMove.h
 * @brief 強敵が動かない状態のクラス
 * @author 茂木翼
 */



#include "BaseStongEnemyState.h"


/// <summary>
/// 強敵が動かない状態
/// </summary>
class StrongEnemyNoneMove : public BaseStongEnemyState {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::unique_ptr<StrongEnemy>& strongEnemy)override;


private:


};

