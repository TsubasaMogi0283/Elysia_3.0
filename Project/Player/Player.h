#pragma once

/**
 * @file Player.h
 * @brief プレイヤーのクラス
 * @author 茂木翼
 */

#include <memory>
#include <list>


#include "WorldTransform.h"
#include "Model.h"

#include "Stage/Ground/StageRect.h"
#include "AABB.h"
#include "PlayerCollisionToStrongEnemy.h"
#include "PlayerCollisionToNormalEnemyAttack.h"
#include "Light/FlashLight/FlashLight.h"
#include "SpotLight.h"
#include "PlayerCollisionToAudioObject.h"

#pragma region 前方宣言

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// マテリアル
/// </summary>
struct Material;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene;

/// <summary>
/// EllysiaEngine
/// </summary>
namespace Ellysia {
	/// <summary>
	/// 入力クラス
	/// </summary>
	class Input;

};

/// <summary>
/// モデル管理クラス
/// </summary>
class ModelManager;

#pragma endregion

/// <summary>
/// プレイヤーの移動状態
/// </summary>
enum PlayerMoveCondition {
	//動かない
	NonePlayerMove,
	//動く
	OnPlayerMove,
};


/// <summary>
/// プレイヤー
/// </summary>
class Player {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

private:
	/// <summary>
	/// ダメージ
	/// </summary>
	void Damaged();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// ImGui表示用
	/// </summary>
	void DisplayImGui();

public:

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns></returns>
	inline float GetSideSize()const {
		return SIDE_SIZE;
	}

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetWorldPosition()const {
		return worldTransform_.GetWorldPosition();
	}


	/// <summary>
	/// 方向
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetDirection() const{
		return moveDirection_;
	}

	/// <summary>
	/// AABBの取得
	/// </summary>
	inline AABB GetAABB() const{
		return aabb_;
	}


	/// <summary>
	/// 体力を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetHP()const {
		return hp_;
	}

	/// <summary>
	/// ダメージを受けたかどうかを取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsDamaged()const {
		return isDameged_;
	}

	/// <summary>
	/// 懐中電灯を取得
	/// </summary>
	/// <returns></returns>
	inline FlashLight* GetFlashLight()const {
		return flashLight_.get();
	}

public:

	/// <summary>
	/// 持っている鍵の数を増やす
	/// </summary>
	inline void AddHaveKeyQuantity() {
		haveKeyQuantity_++;
	}

	/// <summary>
	/// 今鍵を何個持っているか
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetHavingKey() {
		return haveKeyQuantity_;
	}

	/// <summary>
	/// 動く方向の設定
	/// </summary>
	/// <param name="move"></param>
	inline void SetMoveDirection(const Vector3& moveDirection) {
		this->moveDirection_ = moveDirection;
	}

	/// <summary>
	/// 動きの状態を設定
	/// </summary>
	/// <param name="condition"></param>
	inline void SetMoveCondition(const uint32_t& condition) {
		this->moveCondition_ = condition;
	}

	/// <summary>
	/// 走るかどうか
	/// </summary>
	inline void SetIsDash(const bool& isDash) {
		this->isDash_ = isDash;
	}


	/// <summary>
	/// 操作を受け付けるか受け付けないかの設定
	/// </summary>
	/// <param name="isControll"></param>
	inline void SetIsAbleToControll(const bool& isControll) {
		this->isControll_ = isControll;
	}

	/// <summary>
	/// 通常の敵からの攻撃を受け入れるかどうか
	/// </summary>
	/// <param name="isAccept"></param>
	inline void SetIsAcceptDamegeFromNoemalEnemy(const bool& isAccept) {
		this->isAcceptDamegeFromNoemalEnemy_ = isAccept;
	}

	/// <summary>
	/// 通常の敵の攻撃の受け入れを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAcceptDamegeFromNormalEnemy()const {
		return isAcceptDamegeFromNoemalEnemy_;
	}

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	inline void SetPosition(const Vector3& position) {
		this->worldTransform_.translate = position;
	}

public:

	///// <summary>
	///// 通常の敵の当たり判定
	///// </summary>
	///// <returns></returns>
	//inline PlayerCollisionToNormalEnemyAttack* GetCollisionToNormalEnemy()const {
	//	return colliderToNormalEnemy_.get();
	//}
	//
	///// <summary>
	///// 強敵の当たり判定
	///// </summary>
	///// <returns></returns>
	//inline PlayerCollisionToStrongEnemy* GetCollisionToStrongEnemy()const {
	//	return collisionToStrongEnemy_.get();
	//}
	//
	///// <summary>
	///// オーディオオブジェクトに対しての当たり判定
	///// </summary>
	///// <returns></returns>
	//inline PlayerCollisionToAudioObject* GetCollisionToAudioObject()const {
	//	return collosionToAudioObject_.get();
	//}

	/// <summary>
	/// プレイヤー用のコライダーを取得
	/// </summary>
	/// <returns></returns>
	inline std::vector<BasePlayerCollision*> GetColliders()const{
		std::vector<BasePlayerCollision*> colliders;
		for (const auto& collider : colliders_) {
			colliders.push_back(collider.get());
		}
		return colliders;
	};


	/// <summary>
	/// 懐中電灯の当たり判定
	/// </summary>
	/// <returns></returns>
	inline FlashLightCollision* GetFlashLightCollision()const {
		return flashLight_->GetFanCollision();
	}

private:

	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_={};
	//マテリアル
	Material material_ = {};

	//持っている鍵の数
	//可算なのでQuantity
	uint32_t haveKeyQuantity_ = 0u;

	//動く方向
	Vector3 moveDirection_ = {};

	//AABB
	AABB aabb_ = {};



	//体力
	int32_t hp_ = 0;
	//敵の攻撃に当たった時のタイマー
	int32_t downTime_ = 0;
	//敵の攻撃に当たったかどうか
	bool isDamage_ = false;

	//操作可能かどうか
	bool isControll_ = false;
	//移動状態
	uint32_t moveCondition_ = 0u;
	//ダッシュ
	bool isDash_ = false;




	//攻撃されたか
	bool isDameged_ = false;
	int32_t damagedTime_ = 0;
	bool isAcceptDamegeFromNoemalEnemy_ = false;


	//振動の強さ
	float vibeStrength_ = 0.0f;
	//時間
	float vibeTime_ = 0u;

private:
	//幅のサイズ
	const float SIDE_SIZE = 1.0f;
	//時間変化
	const float DELTA_TIME = 1.0f / 60.0f;

private:
	//入力クラス
	Ellysia::Input* input_ = nullptr;
	//モデル管理クラス
	ModelManager* modelManager_ = nullptr;
private:
	//当たり判定のリスト
	std::list<std::unique_ptr<BasePlayerCollision>> colliders_ = {};
	////当たり判定(通常の敵)
	//std::unique_ptr<PlayerCollisionToNormalEnemyAttack>colliderToNormalEnemy_ = nullptr;
	////当たり判定(一発アウトの敵用)
	//std::unique_ptr<PlayerCollisionToStrongEnemy>collisionToStrongEnemy_ = nullptr;
	////オーディオオブジェクト用
	//std::unique_ptr<PlayerCollisionToAudioObject>collosionToAudioObject_ = nullptr;

	//懐中電灯
	std::unique_ptr<FlashLight>flashLight_ = nullptr;

};