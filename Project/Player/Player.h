#pragma once

/**
 * @file GameManager.h
 * @brief プレイヤーのクラス
 * @author 茂木翼
 */


#include "WorldTransform.h"
#include "Model.h"
#include <memory>
#include "Stage/Ground/StageRect.h"
#include "AABB.h"
#include "PlayerCollisionToStrongEnemy.h"
#include "PlayerCollisionToNormalEnemyAttack.h"
#include "Listener/ListenerForLevelEditor.h"
#include "Light/FlashLight/FlashLight.h"
#include "SpotLight.h"




struct Camera;
struct Material;
class GameScene;
class ObjectManager;


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
	Player() = default;

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

	void Damaged();



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
	inline Vector3 GetDirection() {
		return moveDirection_;
	}

	/// <summary>
	/// AABBの取得
	/// </summary>
	inline AABB GetAABB() {
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
	inline void SetPlayerMoveCondition(const uint32_t& condition) {
		this->moveCondition_ = condition;
	}

	/// <summary>
	/// 走るかどうか
	/// </summary>
	inline void SetIsDash(const bool& isDash) {
		this->isDash_ = isDash;
	}

	/// <summary>
	/// 四隅の取得
	/// </summary>
	/// <param name="stageRect"></param>
	/// <returns></returns>
	inline void SetStageRect(const StageRect& stageRect) {
		this->stageRect_ = stageRect;
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

	
public:

	/// <summary>
	/// 通常の敵の当たり判定
	/// </summary>
	/// <returns></returns>
	inline PlayerCollisionToNormalEnemyAttack* GetCollisionToNormalEnemy()const {
		return colliderToNormalEnemy_.get();
	}

	/// <summary>
	/// 強敵の当たり判定
	/// </summary>
	/// <returns></returns>
	inline PlayerCollisionToStrongEnemy* GetCollisionToStrongEnemy()const {
		return collisionToStrongEnemy_.get();
	}

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

	//ステージの四隅
	StageRect stageRect_ = {};

	//マテリアル
	Material material_ = {};



	//持っている鍵の数
	//可算なのでQuantity
	uint32_t haveKeyQuantity_ = 0u;

	//動く方向
	Vector3 moveDirection_ = {};

	//幅のサイズ
	const float SIDE_SIZE = 1.0f;
	//AABB
	AABB aabb_ = {};




	//体力
	int32_t hp_ = 0;
	//敵の攻撃に当たった時のタイマー
	int32_t downTime_ = 0;
	//敵の攻撃に当たったかどうか
	bool isDamage_ = false;
	//攻撃を受け入れるかどうか
	bool acceptDamage_ = false;

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
	//強さ
	float vibeStrength_ = 0.0f;
	//時間
	float vibeTime_ = 0u;

private:
	//歩くスピード
	const float NORMAL_MOVE_SPEED = 0.1f;
	//走るスピード
	const float DASH_MOVE_SPEED = 0.2f;
	//時間変化
	const float DELTA_TIME = 1.0f / 60.0f;



	int count = 0;

private:


	//当たり判定(通常の敵)
	std::unique_ptr<PlayerCollisionToNormalEnemyAttack>colliderToNormalEnemy_ = nullptr;


	//当たり判定(一発アウトの敵用)
	std::unique_ptr<PlayerCollisionToStrongEnemy>collisionToStrongEnemy_ = nullptr;


	//懐中電灯
	std::unique_ptr<FlashLight>flashLight_ = nullptr;

};

