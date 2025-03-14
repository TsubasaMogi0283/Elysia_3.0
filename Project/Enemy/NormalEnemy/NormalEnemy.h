#pragma once


/**
 * @file NormalEnemy.h
 * @brief 敵のクラス
 * @author 茂木翼
 */

#include <memory>

#include "WorldTransform.h"
#include "Model.h"
#include "Material.h"
#include "Particle3D.h"
#include "DirectionalLight.h"

#include "AABB.h"
#include "Enemy/EnemyCondition.h"
#include "EnemyAttackCollision.h"
#include "EnemyFlashLightCollision.h"
#include "Enemy/BaseEnemy.h"
#include "State/BaseNormalEnemyState.h"

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
namespace Elysia {
	/// <summary>
	/// グローバル変数
	/// </summary>
	class GlobalVariables;
}


#pragma endregion

/// <summary>
/// 敵
/// </summary>
class NormalEnemy : public BaseEnemy {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	NormalEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="position">座標</param>
	/// <param name="speed">スピード</param>
	void Initialize(const uint32_t& modelHandle, const Vector3& position, const Vector3& speed)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="spotLight">スポットライト</param>
	void Draw(const Camera& camera, const SpotLight& spotLight)override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalEnemy()override;

public:
	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="newState">新しい状態</param>
	void ChengeState(std::unique_ptr<BaseNormalEnemyState> newState);

	/// <summary>
	/// 現在の状態を取得
	/// </summary>
	/// <returns>状態名</returns>
	std::string GetCurrentStateName()const {
		return currentStateName_;
	}

	/// <summary>
	/// 前回の状態を取得
	/// </summary>
	/// <returns>状態名</returns>
	std::string GetPreStateName()const {
		return preStateName_;
	};


	/// <summary>
	/// 攻撃しているかどうか
	/// </summary>
	/// <returns>フラグ</returns>
	inline bool GetIsAttack()const {
		return isAttack_;
	}

	/// <summary>
	/// 攻撃したかどうかのおフラグを設定
	/// </summary>
	/// <param name="isAttack">攻撃しているかどうか</param>
	inline void SetIsAttack(const bool& isAttack) {
		this->isAttack_ = isAttack;
	}


	/// <summary>
	/// 現在の状態を取得
	/// </summary>
	/// <returns></returns>
	inline BaseNormalEnemyState* GetCurrentState() {
		return currentState_.get();
	}

	/// <summary>
	/// 攻撃用のコリジョンを取得
	/// </summary>
	/// <returns>コリジョン</returns>
	inline EnemyAttackCollision* GetEnemyAttackCollision() const {
		return attackCollision_.get();
	}

	/// <summary>
	/// 懐中電灯用のコリジョンを取得
	/// </summary>
	/// <returns>コリジョン</returns>
	inline EnemyFlashLightCollision* GetEnemyFlashLightCollision() const {
		return enemyFlashLightCollision_.get();
	}

private:
	/// <summary>
	/// 攻撃をライトで受ける
	/// </summary>
	void Damaged();

	/// <summary>
	/// 絶命
	/// </summary>
	void Dead();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();



private:
	//グローバル変数クラス
	Elysia::GlobalVariables* globalVariables_ = nullptr;
	//グループ名
	const std::string GROUNP_NAME_ = "NormalEnemy";

private:

	//幅
	const float_t RADIUS_ = 2.0f;
	//幅(Vector3)
	const Vector3 RADIUS_INTERVAL_ = { .x = RADIUS_,.y = RADIUS_,.z = RADIUS_ };
	

private:
	//パーティクル
	std::unique_ptr<Elysia::Particle3D> particle_ = {};
	//マテリアル
	Material particleMaterial_ = {};
	//デバッグ用のモデルハンドル
	uint32_t debugModelHandle = 0;

	//振動のオフセット
	float_t shakeOffset_ = 0.05f;
	bool isShake_ = false;

	
	//攻撃状態
	bool isAttack_ = false;
	//状態の名前
	//前回
	std::string preStateName_ = "";
	//現在
	std::string currentStateName_ = "";
	
private:
	//攻撃用の当たり判定
	std::unique_ptr<EnemyAttackCollision> attackCollision_ = nullptr;
	//懐中電灯用の当たり判定
	std::unique_ptr<EnemyFlashLightCollision> enemyFlashLightCollision_ = nullptr;
	//状態
	std::unique_ptr<BaseNormalEnemyState> currentState_ = nullptr;
};