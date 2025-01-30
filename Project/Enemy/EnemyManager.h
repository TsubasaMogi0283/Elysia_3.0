#pragma once
/**
 * @file EnemyManager.h
 * @brief 敵管理クラス
 * @author 茂木翼
 */

#include <memory>
#include <list>
#include <sstream>

#include "Stage/Ground/StageRect.h"
#include "Enemy.h"
#include "StrongEnemy.h"
#include "Audio.h"


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
/// EllysiaEngine
/// </summary>
namespace Ellysia {
	/// <summary>
	/// レベルデータ管理クラス
	/// </summary>
	class LevelDataManager;

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
	/// <param name="normalEnemyModel"></param>
	/// <param name="strongEnemyModel"></param>
	/// <param name="csvPath"></param>
	void Initialize(const uint32_t& normalEnemyModel,const uint32_t &strongEnemyModel, const std::string& csvPath);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera ,const SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

public:

	/// <summary>
	/// 通常の敵の生成
	/// </summary>
	/// <param name="position"></param>
	void GenarateNormalEnemy(const Vector3& position);

	/// <summary>
	/// 強敵を生成
	/// </summary>
	/// <param name="position"></param>
	void GenarateStrongEnemy(const Vector3& position);

	/// <summary>
	/// エネミーを消す処理
	/// </summary>
	void DeleteEnemy();

	/// <summary>
	/// 音を止める
	/// </summary>
	void StopAudio();

public:
	/// <summary>
	/// エネミーのリストを取得
	/// </summary>
	/// <returns></returns>
	inline std::list< std::unique_ptr<Enemy>> GetEnemyes() const{
		return enemyes_;
	}
	
	/// <summary>
	/// 強敵の取得
	/// </summary>
	/// <returns>リスト</returns>
	inline std::list< std::unique_ptr<StrongEnemy>>GetStrongEnemyes()const {
		return strongEnemyes_;
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
	inline void SetLevelDataManager(Ellysia::LevelDataManager* levelDataManager, uint32_t levelDataHandle) {
		this->levelDataManager_ = levelDataManager;
		this->levelDataHandle_ = levelDataHandle;
	}

private:
	//プレイヤー
	Player* player_ = nullptr;
	//接近BGM用
	Ellysia::Audio* audio_ = nullptr;
	//ハンドル
	uint32_t audioHandle_ = 0u;
	//レベルデータ管理クラス
	Ellysia::LevelDataManager* levelDataManager_ = nullptr;
	//レベルデータのハンドル
	uint32_t levelDataHandle_ = 0u;

private:
	//前方にいるかどうかの内積
	const float FRONT_DOT = 0.7f;
	//追跡開始の距離
	const float STRONG_ENEMY_TRACKING_START_DISTANCE_ = 30.0f;

private:
	//エネミーのリスト
	//通常
	std::list<std::unique_ptr<Enemy>>enemyes_ = {};
	//強敵
	std::list< std::unique_ptr<StrongEnemy>>strongEnemyes_ = {};

	//モデルハンドル
	//通常
	uint32_t normalEnemyModelHandle_ = 0u;
	//強敵
	uint32_t strongEnemyModelHandle_ = 0u;


	//生成の文字列を入れる
	std::stringstream enemyPositionsFromCSV_;

	

};

