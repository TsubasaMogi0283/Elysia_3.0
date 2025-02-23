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
#include "EnemyCondition.h"
#include "EnemyAttackCollision.h"
#include "EnemyFlashLightCollision.h"
#include "BaseEnemy.h"

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
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera, const SpotLight& spotLight)override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalEnemy()override;



public:



	/// <summary>
	/// 生きているかのフラグを取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsAlive() const {
		return isAlive_;
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
	inline bool GetIsDeleted()const {
		return isDeleted_;
	}


	/// <summary>
	/// 攻撃用の当たり判定を取得
	/// </summary>
	/// <returns></returns>
	inline EnemyAttackCollision* GetEnemyAttackCollision() const {
		return attackCollision_.get();
	}



	/// <summary>
	/// 懐中電灯用の当たり判定を取得
	/// </summary>
	/// <returns></returns>
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
	Ellysia::GlobalVariables* globalVariables_ = nullptr;
	//グループ名
	const std::string GROUNP_NAME_ = "NormalEnemy";

private:

	//幅
	const float RADIUS_ = 1.0f;
	//幅(Vector3)
	const Vector3 RADIUS_INTERVAL_ = { .x = RADIUS_,.y = RADIUS_,.z = RADIUS_ };
	//0に戻る時間
	const int32_t RETURN_ATTCK_TIME_ = 240u;


private:
	//パーティクル
	std::unique_ptr<Ellysia::Particle3D> particle_ = {};
	//マテリアル
	Material particleMaterial_ = {};
	//デバッグ用のモデルハンドル
	uint32_t debugModelHandle = 0;

	//振動のオフセット
	float shakeOffset_ = 0.05f;
	bool isShake_ = false;

	//攻撃
	int32_t attackTime_ = 0;
	bool isAttack_ = false;

private:
	//攻撃用の当たり判定
	std::unique_ptr<EnemyAttackCollision> attackCollision_ = nullptr;
	//懐中電灯用の当たり判定
	std::unique_ptr<EnemyFlashLightCollision> enemyFlashLightCollision_ = nullptr;

};