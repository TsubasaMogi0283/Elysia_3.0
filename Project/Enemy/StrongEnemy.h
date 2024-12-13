#pragma once

/**
 * @file StrongEnemy.h
 * @brief 強敵のクラス
 * @author 茂木翼
 */


#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

#include "Enemy/EnemyCondition.h"
#include "Collider/Collider.h"


struct Camera;
struct SpotLight;

/// <summary>
/// 強敵
/// </summary>
class StrongEnemy :public Collider {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StrongEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	void Initialize(const uint32_t& modelHandle,const Vector3 &position,const Vector3 &speed);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StrongEnemy()=default;


public:
	/// <summary>
	/// 接触
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 非接触
	/// </summary>
	void OffCollision()override;


	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override {
		return worldTransform_.GetWorldPosition();
	}


	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(const Vector3& position) {
		this->playerPosition_ = position;
	}

	inline bool GetIsTouchPlayer()const {
		return isTouchPlayer_;
	}


public:


	/// <summary>
	/// 状態の設定
	/// </summary>
	/// <param name="condition"></param>
	inline void SetCondition(const uint32_t& condition) {
		this->condition_ = condition;
	}

	inline uint32_t GetCondition() const {
		return condition_;
	}



	inline void InvertSpeedX() {
		speed_.x *= -1.0f;
	}
	inline void InvertSpeedZ() {
		speed_.z *= -1.0f;
	}


	inline void SaveSpeed() {
		preSpeed_ = speed_;
	}



	inline AABB GetAABB() {
		return aabb_;
	}


	/// <summary>
	/// 追跡開始距離
	/// </summary>
	/// <param name="distance"></param>
	inline void SetTrackingStartDistance(const float& distance) {
		this->trackingStartDistance_ = distance;
	}



private:
	//状態
	uint32_t condition_ = EnemyCondition::Move;

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};

	//方向
	Vector3 direction_ = {};

	//移動速度
	Vector3 preSpeed_ = {};
	Vector3 speed_ = {};



	AABB aabb_ = {};


	//プレイヤーに当たったかどうか
	bool isTouchPlayer_ = false;
	//プレイヤーの座標
	Vector3 playerPosition_ = {};



	//追跡
	bool isTracking_ = false;

	//追跡開始距離
	float trackingStartDistance_ = 0.0f;
	
};

