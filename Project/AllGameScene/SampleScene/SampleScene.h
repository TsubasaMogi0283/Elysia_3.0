#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

#include <memory>
#include <Particle3D.h>
#include <Audio.h>
#include "BackText.h"
#include "LuminanceBasedOutline.h"
#include "DepthBasedOutline.h"
#include "RadialBlur.h"
#include "Dissolve.h"
#include "RandomEffect.h"
#include "SkinCluster.h"
#include "../../../Ellysia/Polygon/3D/SkyBox/SkyBox.h"
#include "Material.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <GrayScale.h>
#include <SepiaScale.h>
#include <Vignette.h>
#include <BoxFilter.h>
#include <GaussianFilter.h>
#include <Player/Player.h>
#include <Enemy/Enemy.h>
#include <LightWeapon/LightWeapon.h>
#include <Collider/CollisionManager.h>
#include <Key/Key.h>
#include <Stage/Ground/Ground.h>

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	
	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

#pragma region 描画

	/// <summary>
	/// ポストエフェクト掛ける前のスプライト
	/// </summary>
	void DrawSpriteBack()override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;


	void PreDrawPostEffectFirst()override;
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// デストラクタ
	~SampleScene();


private:

	void GenarateEnemy();

	void CheckCollision(std::list<Enemy*>& enemies);

	/// <summary>
	/// 鍵の取得の処理
	/// </summary>
	void KeyCollision();


private:


	std::unique_ptr<Model> model_ = nullptr;
	uint32_t modelHandle = 0;
	WorldTransform worldTransform_ = {};
	
	Camera camera_ = {};

	DirectionalLight directionalLight_ = {};
	SpotLight spotLight_ = {};

	Material material_ = {};

	const char* GroupName = "Player";
	Vector3 position = {};

	std::unique_ptr<BackText> back_ = nullptr;



	Vector3 cameraPosition_ = {};
	Vector3 CAMERA_POSITION_OFFSET = { 0.0f,1.0f,0.0f };

	Vector3 cameraThirdPersonViewOfPointPosition_ = {};
	Vector3 thirdPersonViewOfPointRotate_ = {};


	//プレイヤー
	std::unique_ptr<Player>player_ = nullptr;

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//鍵
	std::list<Key*> keyes_ = {};

	
	//敵
	std::list <Enemy*> enemys_ = {};
	uint32_t enemyModelHandle_ = 0;

	//プレイヤーのライトの判定
	LightWeapon* lightCollision_ = nullptr;
	WorldTransform lightCollisionWorldTransform_ = {};


	Vector3 lightPosition = {};
	Vector3 lightDirection_ = {};

	float theta_ = 0.0f;
	float originPhi_ = 0.0f;
	float decay_ = 0.0f;
	float fallOff_ = 0.0f;
	float cosAngle_ = 0.0f;
	float intencity_ = 4.0f;
	float distance_ = 10.0f;

	uint32_t viewOfPoint_ = 0;



	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;







#pragma region デバッグ用のオブジェクト

	//スカイボックス
	std::unique_ptr<SkyBox> skyBox_ = nullptr;
	WorldTransform skyBoxWorldTransform_ = {};


	std::unique_ptr<Model> debugTower_ = nullptr;
	WorldTransform debugTowerWorldTransform_ = {};

#pragma endregion


};