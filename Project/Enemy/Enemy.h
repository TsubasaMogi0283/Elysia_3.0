#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "../Collider/Collider.h"
#include "Material.h"
#include <memory>
#include "../../Ellysia/Line/Line.h"
struct Camera;
struct SpotLight;
class Player;

//後々StatePatternで分けるつもり
enum EnemyCondition {
	//何もしない
	NoneMove,
	//通常
	Move,
	//追いかけ始めるときの座標
	PreTracking,
	//追いかける
	Tracking,
	//攻撃
	Attack,
};

class Enemy :public Collider{
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
	void Draw(Camera& camera,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() = default;



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
	/// 座標の指定
	/// </summary>
	/// <param name="translate"></param>
	void SetTranslate(Vector3& translate);

#pragma region 座標の設定

	/// <summary>
	/// X座標の設定
	/// </summary>
	/// <param name="posX"></param>
	inline void SetPositionX(float& posX) {
		this->worldTransform_.translate_.x = posX;
	}

	/// <summary>
	/// Y座標の設定
	/// </summary>
	/// <param name="posX"></param>
	inline void SetPositionY(float& posY) {
		this->worldTransform_.translate_.y = posY;
	}

	/// <summary>
	/// Z座標の設定
	/// </summary>
	/// <param name="posX"></param>
	inline void SetPositionZ(float& posZ) {
		this->worldTransform_.translate_.z = posZ;
	}

#pragma endregion


#pragma region スピードの反転
	inline void InvertSpeedX() {
		this->speed_.x *= -1.0f;
	}
	inline void InvertSpeedY() {
		this->speed_.y *= -1.0f;
	}
	inline void InvertSpeedZ() {
		this->speed_.z *= -1.0f;
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

#pragma region EnemyManagerから取得

	/// <summary>
	/// プレイヤーの半径
	/// </summary>
	/// <param name="radius"></param>
	void SetPlayerRadius(float& radius) {
		this->playerRadius_ = radius;
	}

	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerPosition(Vector3& position) {
		this->playerPosition_ = position;
	}
#pragma endregion



public:
	

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

private:
	uint32_t preCondition_ = EnemyCondition::NoneMove;
	uint32_t condition_ = EnemyCondition::Move;



private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	const float SCALE_SIZE = 8.0f;
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
	float t_ = 0.0f;

	//攻撃
	int32_t attackTime_ = 0;

	//プレイヤーの座標
	Vector3 playerPosition_ = {};
	//プレイヤーの半径
	float playerRadius_ = 0.0f;





	//モデル
	std::unique_ptr<Model> debugModel_ = nullptr;
	WorldTransform debugModelWorldTransform_ = {};
};