#pragma once
/**
 * @file PlayerCollisionToStageObject.h
 * @brief ステージオブジェクトに対するCollisionクラス
 * @author 茂木翼
 */

#include "BasedPlayerCollision.h"


/// <summary>
/// ステージオブジェクトに対するCollision
/// </summary>
class PlayerCollisionToStageObject :public BasedPlayerCollision {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerCollisionToStageObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision()override;


	/// <summary>
	/// 非衝突
	/// </summary>
	void OffCollision()override;


	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;


	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerCollisionToStageObject() = default;


public:

	/// <summary>
	/// ワールド座標を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(const Vector3& position)override {
		this->position_ = position;
	}

	


private:
	//位置
	Vector3 position_ = {};

	//衝突
	bool isTouch_ = false;

};

