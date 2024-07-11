#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "../Collider/Collider.h"
#include "Material.h"
#include <memory>
struct Camera;
struct SpotLight;
class Player;

class Enemy :public Collider{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	void Initialize(uint32_t modelHandle, Vector3 position, Vector3 speed);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy()=default;



public:
	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 座標の指定
	/// </summary>
	/// <param name="translate"></param>
	void SetTranslate(Vector3 translate);

public:
	

	/// <summary>
	/// 生きているかのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() {
		return isAlive_;
	}


	/// <summary>
	/// プレイヤー
	/// </summary>
	/// <param name="player"></param>
	inline void Setlayer(Player* player) {
		this->player_ = player;
	}


private:
	//プレイヤー
	Player* player_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	Vector3 speed_ = {};

	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	
	//マテリアル
	Material material_ = {};
	Vector4 color_ = {};
	
	//消滅
	int32_t deleteTime_ = 0;
	bool isAlive_ = true;

	//追跡
	bool isTracking_ = false;
};

