#pragma once
/**
 * @file EnemyManager.h
 * @brief 敵管理クラス
 * @author 茂木翼
 */

#include <memory>
#include <list>
#include <vector>
#include <sstream>

#include "BaseEnemy.h"
#include "NormalEnemy/NormalEnemy.h"
#include "StrongEnemy/StrongEnemy.h"



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
/// ElysiaEngine
/// </summary>
namespace Elysia {
	/// <summary>
	/// レベルデータ管理クラス
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;
}

#pragma endregion

/// <summary>
/// 敵管理クラス
/// </summary>
class EnemyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager();
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="normalEnemyModel">通常の敵のモデルハンドル</param>
	/// <param name="strongEnemyModel">強敵のモデルハンドル</param>
	/// <param name="normalEnemyPositions">通常の敵の座標</param>
	/// <param name="strongEnemyPositions">強敵の座標</param>
	void Initialize(const uint32_t& normalEnemyModel,const uint32_t &strongEnemyModel, const std::vector<Vector3>& normalEnemyPositions, const std::vector<Vector3>& strongEnemyPositions);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="spotLight">スポットライト</param>
	void DrawObject3D(const Camera& camera ,const SpotLight& spotLight);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager()=default;

public:

	/// <summary>
	/// 通常の敵の生成
	/// </summary>
	/// <param name="position">座標</param>
	void GenerateNormalEnemy(const Vector3& position);

	/// <summary>
	/// 強敵を生成
	/// </summary>
	/// <param name="position">座標</param>
	void GenerateStrongEnemy(const Vector3& position);

	/// <summary>
	/// エネミーを消す処理
	/// </summary>
	void DeleteEnemy();

	/// <summary>
	/// 音を止める
	/// </summary>
	void StopAudio();


private:
	/// <summary>
	/// ImGui表示用
	/// </summary>
	void DisplayImGui();


public:
	/// <summary>
	/// エネミーを取得
	/// </summary>
	/// <returns></returns>
	inline std::vector<NormalEnemy*> GetNormalEnemies() const{
		std::vector<NormalEnemy*> enemies;
		for (const auto& enemy : enemies_) {
			enemies.push_back(enemy.get()); 
		}
		return enemies;
	}
	
	/// <summary>
	/// 強敵の取得
	/// </summary>
	/// <returns>リスト</returns>
	inline std::vector<StrongEnemy*>GetStrongEnemies()const {
		std::vector<StrongEnemy*> enemies;
		for (const auto& enemy : strongEnemies_) {
			enemies.push_back(enemy.get());
		}
		return enemies;
	}

	/// <summary>
	/// 通常の敵の最短距離を取得
	/// </summary>
	/// <returns></returns>
	inline float_t GeClosestNormalEnemyDistance()const {
		return closestNormalEnemyDistance_;
	}

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	inline void SetPlayer(Player* player) {
		this->player_=player;
	}
	
	/// <summary>
	/// レベルデータ管理クラスとハンドルの設定
	/// </summary>
	/// <param name="levelDataManager">レベルデータ管理クラス</param>
	/// <param name="levelDataHandle">ハンドル</param>
	inline void SetLevelDataManager(Elysia::LevelDataManager* levelDataManager,const uint32_t& levelDataHandle) {
		this->levelDataManager_ = levelDataManager;
		this->levelDataHandle_ = levelDataHandle;
	}

private:
	//プレイヤー
	Player* player_ = nullptr;
	//接近BGM用
	Elysia::Audio* audio_ = nullptr;
	//ハンドル
	uint32_t audioHandle_ = 0u;
	//レベルデータ管理クラス
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//レベルデータのハンドル
	uint32_t levelDataHandle_ = 0u;

private:
	//前方にいるかどうかの内積
	const float_t FRONT_DOT_ = 0.7f;
	//追跡開始の距離
	const float_t STRONG_ENEMY_TRACKING_START_DISTANCE_ = 30.0f;


private:
	/// <summary>
	/// 最短の情報
	/// </summary>
	struct ClosestEnemyInformation {
		//座標
		Vector3 position;
		//方向
		Vector3 direction;

	};

private:
	//エネミーのリスト
	//通常
	std::list<std::unique_ptr<NormalEnemy>>enemies_ = {};
	//強敵
	std::list<std::unique_ptr<StrongEnemy>>strongEnemies_ = {};

	//モデルハンドル
	//通常
	uint32_t normalEnemyModelHandle_ = 0u;
	//強敵
	uint32_t strongEnemyModelHandle_ = 0u;

	// 最短距離と敵の座標のペアを格納する
	std::vector<std::pair<float_t, ClosestEnemyInformation>> enemyDistancePairs;
	//最短距離
	float_t closestNormalEnemyDistance_ = 0.0f;
	//最短情報
	ClosestEnemyInformation closestEnemyInformation_ = {};
	//内積
	float_t playerAndNormalEnemyDot_ = 0.0f;
};

