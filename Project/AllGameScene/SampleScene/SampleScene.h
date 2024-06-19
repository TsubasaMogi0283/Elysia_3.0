#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

#include <memory>
#include <list>
#include <Particle3D.h>
#include <Audio.h>
#include "SkinCluster.h"
#include "../../Enemy/Enemy.h"
#include "../../Collider/CollisionManager.h"
#include "../../LightWeapon/LightWeapon.h"
#include "../../Player/Player.h"

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;



private:

	void GenarateEnemy();

	void CheckCollision(std::list<Enemy*>& enemies);

private:

	

private:
	Camera camera_ = {};

	//プレイヤー
	std::unique_ptr<Player>player_ = nullptr;

	//地面
	std::unique_ptr<Model> ground_ = nullptr;
	WorldTransform groundWorldTransform_ = {};

	//敵
	std::list <Enemy*> enemys_ = {};
	uint32_t enemyModelHandle_ = 0;

	//プレイヤーのライトの判定
	LightWeapon* lightCollision_ = nullptr;
	WorldTransform lightCollisionWorldTransform_ = {};


	Vector3 lightPosition = {};
	Vector3 lightDirection_ = {};

	float theta = 0.0f;
	float decay_ = 0.0f;
	float fallOff_ = 0.0f;
	float cosAngle_ = 0.0f;
	float intencity_ = 4.0f;
	float distance_ = 10.0f;
	const char* GroupName = "Player";

	uint32_t viewOfPoint_ = 0;



	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

};