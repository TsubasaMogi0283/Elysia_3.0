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

struct Camera;
struct SpotLight;
class Player;

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
	void Initialize(uint32_t modelHandle, Vector3 position, Vector3 speed);

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
	~Enemy();



public:
	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

#pragma region 座標の設定

	/// <summary>
	/// Z座標の設定
	/// </summary>
	/// <param name="posX"></param>
	inline void SetPositionZ(float& posZ) {
		this->worldTransform_.translate.z = posZ;
	}

#pragma endregion



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
	void SetPlayerPosition(Vector3& position) {
		this->playerPosition_ = position;
	}

	/// <summary>
	/// AABBの取得
	/// </summary>
	/// <returns></returns>
	inline AABB GetAABB() {
		return aabb_;
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


#pragma region 攻撃用

	//攻撃用
	inline EnemyAttackCollision* GetEnemyAttackCollision() {
		return attackCollision_;
	}


#pragma endregion



#pragma region 懐中電灯

	//懐中電灯用の当たり判定
	inline EnemyFlashLightCollision* GetEnemyFlashLightCollision() {
		return enemyFlashLightCollision_;
	}

#pragma endregion

	



private:
	uint32_t preCondition_ = EnemyCondition::NoneMove;
	uint32_t condition_ = EnemyCondition::Move;



private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	const float SCALE_SIZE = 10.0f;
	//移動速度
	Vector3 preSpeed_ = {};
	Vector3 speed_ = {};

	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	
	//マテリアル
	Material material_ = {};
	Vector4 color_ = {};
	
	//消滅
	int32_t deleteTime_ = 0;
	bool isAlive_ = true;

	//追跡
	bool isTracking_ = false;
	Vector3 preTrackingPosition_ = {};
	Vector3 preTrackingPlayerPosition_ = {};

	//向き
	Vector3 direction_ = {};

	//攻撃
	int32_t attackTime_ = 0;

	//AABB
	AABB aabb_ = {};


	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	//モデル
	std::unique_ptr<Model> debugModel_ = nullptr;
	WorldTransform debugModelWorldTransform_ = {};

	//攻撃用の当たり判定
	EnemyAttackCollision* attackCollision_ = nullptr;
	//懐中電灯用の当たり判定
	EnemyFlashLightCollision* enemyFlashLightCollision_ = nullptr;

};