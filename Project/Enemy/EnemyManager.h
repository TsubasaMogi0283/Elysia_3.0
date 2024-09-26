#pragma once
#include "Enemy.h"
#include <memory>
#include <list>

#include "Stage/Ground/StageRect.h"
#include "StrongEnemy.h"



struct Camera;
struct SpotLight;
class Player;
class ObjectManager;


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
	void Initialize(uint32_t& normalEnemyModel, uint32_t &strongEnemyModel);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera ,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

public:
	/// <summary>
	/// エネミーのリストを取得
	/// </summary>
	/// <returns></returns>
	inline std::list<Enemy*> GetEnemyes() {
		return enemyes_;
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
	Player* player_ = nullptr;
	ObjectManager* objectManager_ = nullptr;

	//エネミーのリスト
	std::list<Enemy*>enemyes_ = {};
	std::list<StrongEnemy*>strongEnemyes_ = {};



	//モデルハンドル
	uint32_t normalEnemyModelHandle_ = 0u;
	uint32_t strongEnemyModelHandle_ = 0u;

	const float ENEMY_SCALE_SIZE_ = 1.0f;
	


	

	AABB aabb[2] = {};

	Vector3 enemyPosition[2] = {};

	StageRect stageRect_ = {};

	//モデル
	Material material_ = {};
};

