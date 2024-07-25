#pragma once
#include "Enemy.h"
#include <memory>
#include <list>

#include "Stage/Ground/StageRect.h"

struct Camera;
struct SpotLight;
class Player;

class EnemyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelhandle"></param>
	void Initialize(uint32_t modelhandle);
	
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
	/// エネミーを消す処理
	/// </summary>
	void DeleteEnemy();
		
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) {
		this->player_=player;
	}
	
	/// <summary>
	/// ステージの四隅を取得
	/// </summary>
	/// <param name="stageRect"></param>
	void SetStageRectangle(StageRect& stageRect) {
		this->stageRect_ = stageRect;
	}

private:
	Player* player_ = nullptr;

	//エネミーのリスト
	std::list<Enemy*>enemyes_ = {};
	//モデルハンドル
	uint32_t modelHandle_ = 0;


	float z =0.0f;
	//float dot = 0.0f;
	

	StageRect stageRect_ = {};

	//モデル
	std::unique_ptr<Model> debugModel_ = nullptr;
	WorldTransform debugModelWorldTransform_ = {};
	Material material_ = {};
};

