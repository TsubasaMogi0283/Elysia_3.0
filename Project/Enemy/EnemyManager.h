#pragma once
/**
 * @file EnemyManager.h
 * @brief 敵管理クラス
 * @author 茂木翼
 */

#include <memory>
#include <list>

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
/// オブジェクト管理クラス
/// </summary>
class ObjectManager;

#pragma endregion

/// <summary>
/// 敵管理クラス
/// </summary>
class EnemyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="normalEnemymodel"></param>
	void Initialize(const uint32_t& normalEnemyModel,const uint32_t &strongEnemyModel);
	
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
	/// エネミーのリストを取得
	/// </summary>
	/// <returns></returns>
	inline std::list<Enemy*> GetEnemyes() const{
		return enemyes_;
	}
	
	/// <summary>
	/// 強敵の取得
	/// </summary>
	/// <returns></returns>
	inline std::list<StrongEnemy*>GetStrongEnemyes()const {
		return strongEnemyes_;
	}



	/// <summary>
	/// エネミーの生成
	/// </summary>
	void GenarateEnemy();


	/// <summary>
	/// 強い敵を生成
	/// </summary>
	void GenarateStrongEnemy();

	/// <summary>
	/// エネミーを消す処理
	/// </summary>
	void DeleteEnemy();
	
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player"></param>
	inline void SetPlayer(Player* player) {
		this->player_=player;
	}
	
	/// <summary>
	/// オブジェクト管理クラスの設定
	/// </summary>
	/// <param name="objectManager"></param>
	inline void SetObjectManager(ObjectManager* objectManager) {
		this->objectManager_ = objectManager;
	}

	/// <summary>
	/// ステージの四隅を取得
	/// </summary>
	/// <param name="stageRect"></param>
	inline void SetStageRectangle(StageRect& stageRect) {
		this->stageRect_ = stageRect;
	}

	



private:
	//前方にいるかどうかの内積
	const float FRONT_DOT = 0.7f;

	//追跡開始の距離
	const float STRONG_ENEMY_TRACKING_START_DISTANCE_ = 30.0f;

private:
	//プレイヤー
	Player* player_ = nullptr;
	//オブジェクト管理クラス
	ObjectManager* objectManager_ = nullptr;

	//エネミーのリスト
	//通常
	std::list<Enemy*>enemyes_ = {};
	//強敵
	std::list<StrongEnemy*>strongEnemyes_ = {};

	

	//モデルハンドル
	//通常
	uint32_t normalEnemyModelHandle_ = 0u;
	//強敵
	uint32_t strongEnemyModelHandle_ = 0u;

	//ステージの四隅
	StageRect stageRect_ = {};



	
	//接近BGM用
	Audio* audio_ = nullptr;
	//ハンドル
	uint32_t audioHandle_ = 0u;

};

