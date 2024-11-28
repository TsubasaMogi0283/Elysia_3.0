#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Material.h"
#include <memory>
#include "Stage/Ground/StageRect.h"
#include "EnemyAttackCollision.h"
#include "AABB.h"
#include "EnemyCondition.h"
#include "EnemyFlashLightCollision.h"
#include "Particle3D.h"


struct Camera;
struct SpotLight;
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	void Initialize(const uint32_t& modelHandle,const Vector3& position,const Vector3& speed);

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
	~Enemy()=default;



public:
	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetWorldPosition()const {
		return worldTransform_.GetWorldPosition();
	};

	/// <summary>
	/// AABBの取得
	/// </summary>
	/// <returns></returns>
	inline AABB GetAABB()const {
		return aabb_;
	}

	/// <summary>
	/// Z座標の設定
	/// </summary>
	/// <param name="posX"></param>
	inline void SetPositionZ(const float& posZ) {
		this->worldTransform_.translate.z = posZ;
	}


	/// <summary>
	/// 生きているかのフラグを取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsAlive() const{
		return isAlive_;
	}

	/// <summary>
	/// 向きを取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDirection() const {
		return direction_;
	}

	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPlayerPosition(const Vector3& position) {
		this->playerPosition_ = position;
	}
	
	/// <summary>
	/// 前の状態の設定
	/// </summary>
	/// <param name="condition"></param>
	inline void SetPreCondition(const uint32_t& preCondition) {
		this->preCondition_ = preCondition;
	}

	/// <summary>
	/// 前の状態を取得
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetPreCondition() const {
		return preCondition_;
	}

	/// <summary>
	/// 状態の設定
	/// </summary>
	/// <param name="condition"></param>
	inline void SetCondition(const uint32_t& condition) {
		this->condition_ = condition;
	}

	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetCondition() const {
		return condition_;
	}

	/// <summary>
	/// X軸反転
	/// </summary>
	inline void InvertSpeedX() {
		this->speed_.x *= -1.0f;
	}
	/// <summary>
	/// Z軸反転
	/// </summary>
	inline void InvertSpeedZ() {
		this->speed_.z *= -1.0f;
	}

	/// <summary>
	/// 止まる前にスピードを記録する
	/// </summary>
	inline void SaveSpeed() {
		this->preSpeed_ = this->speed_;
	}

	/// <summary>
	/// 再度動くとき
	/// </summary>
	inline void MoveAgain() {
		this->speed_ = this->preSpeed_;
	}


	/// <summary>
	/// 攻撃しているかどうか
	/// </summary>
	/// <returns></returns>
	inline bool GetIsAttack()const {
		return isAttack_;
	}


	/// <summary>
	/// 消すかどうか
	/// </summary>
	inline bool GetIsDeleted()const  {
		return isDeleted_;
	}


	/// <summary>
	/// 攻撃用の当たり判定を取得
	/// </summary>
	/// <returns></returns>
	inline EnemyAttackCollision* GetEnemyAttackCollision() const{
		return attackCollision_.get();
	}



	/// <summary>
	/// 懐中電灯用の当たり判定を取得
	/// </summary>
	/// <returns></returns>
	inline EnemyFlashLightCollision* GetEnemyFlashLightCollision() const{
		return enemyFlashLightCollision_.get();
	}

private:
	/// <summary>
	/// 攻撃をライトで受ける
	/// </summary>
	void Damaged();



private:
	//状態
	uint32_t preCondition_ = EnemyCondition::NoneMove;
	uint32_t condition_ = EnemyCondition::Move;



private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//スケールサイズ
	const float SCALE_SIZE = 10.0f;
	//移動速度
	Vector3 preSpeed_ = {};
	Vector3 speed_ = {};

	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	
	//マテリアル
	Material material_ = {};
	
	//生存
	bool isAlive_ = true;

	//AABB
	AABB aabb_ = {};
	//幅
	const float RADIUS_ = 1.0f;
	//幅(Vector3)
	const Vector3 RADIUS_INTERVAL_ = { .x = RADIUS_,.y = RADIUS_,.z = RADIUS_ };

	//消滅
	//時間とフラグ
	int32_t deleteTime_ = 0;
	bool isDeleted_ = false;

	//消えるときのパーティクル
	std::unique_ptr<Particle3D>particle_ = nullptr;

	//追跡
	bool isTracking_ = false;
	//追跡前の座標
	Vector3 preTrackingPosition_ = {};
	Vector3 preTrackingPlayerPosition_ = {};

	//向き
	Vector3 direction_ = {};

	//攻撃
	int32_t attackTime_ = 0;
	bool isAttack_ = false;

	//プレイヤーの座標
	Vector3 playerPosition_ = {};


	//攻撃用の当たり判定
	std::unique_ptr<EnemyAttackCollision> attackCollision_ = nullptr;
	//懐中電灯用の当たり判定
	std::unique_ptr<EnemyFlashLightCollision> enemyFlashLightCollision_ = nullptr;

};