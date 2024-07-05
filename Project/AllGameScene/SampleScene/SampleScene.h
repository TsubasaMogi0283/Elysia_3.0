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
#include <Key/KeyManager.h>
#include <Light/FlashLight/FlashLight.h>
#include <Enemy/EnemyManager.h>
#include <Stage/Gate/Gate.h>
#include <Fan.h>
#include "Collision.h"

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

	
	void CheckCollision(std::list<Enemy*>& enemies);

	/// <summary>
	/// 鍵の取得の処理
	/// </summary>
	void KeyCollision();





private:

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};
	Vector3 CAMERA_POSITION_OFFSET = { 0.0f,1.0f,0.0f };

	Vector3 cameraThirdPersonViewOfPointPosition_ = {};
	Vector3 thirdPersonViewOfPointRotate_ = {};

	bool isRotateYKey_ = false;
	bool isRotateXKey_ = false;


	//平行光源
	DirectionalLight directionalLight_ = {};
	//スポットライト
	std::unique_ptr<FlashLight> flashLight_ = nullptr;
	//マテリアル
	Material material_ = {};

	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;



	

	//プレイヤー
	std::unique_ptr<Player>player_ = nullptr;
	Vector3 playerDirection_ = {0.0f,0.0f,0.0f};
	bool isPlayerMoveKey_ = false;

	uint32_t bTriggerTime_ = 0;
	bool isBTrigger_ = false;

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	bool isEscape_ = false;

	//鍵
	std::unique_ptr<KeyManager> keyManager_ = {};
	uint32_t keyQuantity_ = 0;

	
	//敵
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	uint32_t enemyModelHandle_ = 0;

	//プレイヤーのライトの判定
	LightWeapon* lightCollision_ = nullptr;
	WorldTransform lightCollisionWorldTransform_ = {};

	uint32_t viewOfPoint_ = 0;

	float theta_ = 0.0f;
	float originPhi_ = 0.0f;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;







#pragma region デバッグ用のオブジェクト
	Vector3 cameraTranslate = {};

	std::unique_ptr<Model> debugTower_ = nullptr;
	WorldTransform debugTowerWorldTransform_ = {};

	std::unique_ptr<Model> debugFanCollisionSphereModel_ = nullptr;
	WorldTransform debugFanCollisionSphereWorldTransform_ = {};
	Material debugFanCollisionSphereMaterial_ = {};

#pragma endregion




};