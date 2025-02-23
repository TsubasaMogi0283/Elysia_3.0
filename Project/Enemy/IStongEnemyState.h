#pragma once

/**
 * @file IStongEnemyState.h
 * @brief 強敵の行動パターンのインターフェイス
 * @author 茂木翼
 */

/// <summary>
/// 強敵の行動パターン
/// </summary>
class IStongEnemyState {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


protected:



};