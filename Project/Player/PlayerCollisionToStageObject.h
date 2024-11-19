#pragma once
#include "Collider/Collider.h"


class PlayerCollisionToStageObject :public Collider {
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
	inline void SetPlayerGetWorldPosition(const Vector3& position) {
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

