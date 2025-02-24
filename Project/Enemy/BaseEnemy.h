#pragma once
/**
 * @file BaseEnemy.h
 * @brief 敵の基底クラス
 * @author 茂木翼
 */

#include <memory>

#include "WorldTransform.h"
#include "Model.h"
#include "Material.h"
#include "AABB.h"
#include "Particle3D.h"

#include "EnemyCondition.h"

#pragma region 前方宣言

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

/// <summary>
/// プレイヤー
/// </summary>
class Player;

/// <summary>
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Ellysia {
	/// <summary>
	/// グローバル変数
	/// </summary>
	class GlobalVariables;
}


#pragma endregion

/// <summary>
/// 敵の基底クラス
/// </summary>
class BaseEnemy{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="position">座標</param>
	/// <param name="speed">スピード</param>
	virtual void Initialize(const uint32_t& modelHandle, const Vector3& position, const Vector3& speed) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="spotLight"></param>
	virtual void Draw(const Camera& camera, const SpotLight& spotLight) = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseEnemy()=default;



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
	/// スピードの設定
	/// </summary>
	/// <param name="speed"></param>
	inline void SetSpeed(Vector3& speed) {
		this->speed_ = speed;
	}

	/// <summary>
	/// 消すかどうか
	/// </summary>
	inline bool GetIsDeleted()const  {
		return isDeleted_;
	}

protected:
	//状態
	uint32_t preCondition_ = EnemyCondition::NoneMove;
	uint32_t condition_ = EnemyCondition::Move;

protected:
	
	//モデル
	std::unique_ptr<Ellysia::Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//AABB
	AABB aabb_ = {};

	//マテリアル
	Material material_ = {};

	//移動速度
	Vector3 preSpeed_ = {};
	Vector3 speed_ = {};

	//向き
	Vector3 direction_ = {};
	
	//生存
	bool isAlive_ = true;

	
	
	//消滅
	//時間とフラグ
	int32_t deleteTime_ = 0;
	bool isDeleted_ = false;
	//追跡
	bool isTracking_ = false;
	//追跡前の座標
	Vector3 preTrackingPosition_ = {};
	Vector3 preTrackingPlayerPosition_ = {};
	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	
};