#pragma once
/**
 * @file PlayerCollisionToAudioObject.h
 * @brief オーディオオブジェクトに対してのコライダークラス
 * @author 茂木翼
 */


#include "Collider/Collider.h"

/// <summary>
/// オーディオオブジェクトに対してのコライダー
/// </summary>
class PlayerCollisionToAudioObject:public Collider{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerCollisionToAudioObject() = default;

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
	~PlayerCollisionToAudioObject() = default;


public:

	/// <summary>
	/// ワールド座標を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(const Vector3& position) {
		this->position_ = position;
	}

	/// <summary>
	/// 衝突
	/// </summary>
	/// <returns></returns>
	inline bool GetIsTouch()const {
		return isTouch_;
	}


private:
	//位置
	Vector3 position_ = {};

	//衝突
	bool isTouch_ = false;

};

