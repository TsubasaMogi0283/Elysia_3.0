#pragma once
#include "Enemy.h"
#include <memory>
#include <list>

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
	

private:
	Player* player_ = nullptr;

	//エネミーのリスト
	std::list<Enemy*>enemyes_ = {};
	//モデルハンドル
	uint32_t modelHandle_ = 0;


};

