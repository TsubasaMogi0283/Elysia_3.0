#pragma once

#include <memory>
#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

#include "Enemy/EnemyCondition.h"
#include "Collider/Collider.h"

struct Camera;
struct SpotLight;


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
	void Initialize(uint32_t& modelHandle,Vector3 &position,Vector3 &speed);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera, SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StrongEnemy()=default;


public:
	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;


	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(Vector3& position) {
		this->playerPosition_ = position;
	}

	inline bool GetIsTouchPlayer()const {
		return isTouchPlayer_;
	}


public:
	/// <summary>
	/// 前の状態の設定
	/// </summary>
	/// <param name="condition"></param>
	inline void SetPreCondition(uint32_t& preCondition) {
		this->preCondition_ = preCondition;
	}

	uint32_t GetPreCondition() const {
		return preCondition_;
	}


	/// <summary>
	/// 状態の設定
	/// </summary>
	/// <param name="condition"></param>
	inline void SetCondition(uint32_t& condition) {
		this->condition_ = condition;
	}

	uint32_t GetCondition() const {
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



private:
	//状態
	uint32_t preCondition_ = EnemyCondition::NoneMove;
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
	Vector3 preTrackingPosition_ = {};
	Vector3 preTrackingPlayerPosition_ = {};


};

