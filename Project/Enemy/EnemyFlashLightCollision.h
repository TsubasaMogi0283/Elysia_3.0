#pragma once
#include "Collider/Collider.h"
#include "Vector3.h"

class EnemyFlashLightCollision : public Collider {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyFlashLightCollision() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyFlashLightCollision() = default;

public:


	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision()override;


	/// <summary>
	/// 敵の位置を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetEnemyPosition(const Vector3& position) {
		this->enemyPosition_ = position;
	}

	/// <summary>
	/// 接触しているか
	/// </summary>
	/// <returns></returns>
	inline bool GetIsTouched()const {
		return isTouch_;
	}


private:
	//触れているかどうか
	bool isTouch_ = false;

	//位置
	Vector3 enemyPosition_ = {};


};

