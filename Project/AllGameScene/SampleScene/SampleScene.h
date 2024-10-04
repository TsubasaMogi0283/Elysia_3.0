#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Audio.h"

#include <memory>
#include <Audio.h>
#include "BackText.h"


#include "SkinCluster.h"
#include "Material.h"
#include "SpotLight.h"
#include "DirectionalLight.h"


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
#include <Stage/Skydome/Skydome.h>

#include "Input.h"
#include <Stage/ObjectManager/ObjectManager.h>
#include "../../../Ellysia/Polygon/3D/Sphere/Sphere.h"

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene()=default;

	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
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
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SampleScene();



private:



	/// <summary>
	/// 鍵の取得の処理
	/// </summary>
	void KeyCollision();


	/// <summary>
	/// ステージオブジェクトとの当たり判定
	/// </summary>
	void ObjectCollision();

	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	void PlayerMove();

	void EscapeCondition();

private:
	enum GameCondition {
		GameFadeIn,
		Explanation,
		GamePlay,
		GameFadeOut,
	};

	GameCondition gameCondition_ = GameCondition::GameFadeIn;

private:


	//操作は全部ゲームシーンで統一させたい
	//コマンドパターンですっきりさせても良さそう
	XINPUT_STATE joyState{};

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};
	Vector3 CAMERA_POSITION_OFFSET = { 0.0f,1.0f,0.0f };

	//3人称視点
	Vector3 cameraThirdPersonViewOfPointPosition_ = {};
	Vector3 thirdPersonViewOfPointRotate_ = {};

	bool isRotateYKey_ = false;
	bool isRotateXKey_ = false;


	//スポットライト
	std::unique_ptr<FlashLight> flashLight_ = nullptr;
	//マテリアル
	Material material_ = {};

	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;


	

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	//座標
	Vector3 playerPosition_ = {};
	//向き
	Vector3 playerMoveDirection_ = {};
	//キーボードで動かしているかどうか
	bool isPlayerMoveKey_ = false;
	//動いているかどうか
	bool isPlayerMove_ = false;
	//ダッシュしているかどうか
	bool isPlayerDash_ = false;

	//Bボタンのトリガー
	uint32_t bTriggerTime_ = 0u;
	bool isBTrigger_ = false;

	//オブジェクトマネージャー
	ObjectManager* objectManager_ = nullptr;


	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	bool isEscape_ = false;

	

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	
	
	//敵
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	uint32_t enemyModelHandle_ = 0u;

	//プレイヤーのライトの判定
	LightWeapon* lightCollision_ = nullptr;
	WorldTransform lightCollisionWorldTransform_ = {};

	uint32_t viewOfPoint_ = 0u;

	float theta_ = 0.0f;
	float originPhi_ = 0.0f;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	//強い敵と接触したかどうか
	bool isTouchStrongEnemy_ = false;
	
#pragma region ゲーム中のUI

	//UIManagerを作った方がよさそう
	//ベタ書き過ぎるので

	//UIを表示するかどうか
	bool isDisplayUI_ = false;

	//脱出テキスト
	std::unique_ptr<Sprite> escapeText_ = nullptr;

	//操作
	std::unique_ptr<Sprite> operation_ = nullptr;

	//鍵取得
	std::unique_ptr<Sprite> pickUpKey_ = nullptr;

	//プレイヤーの体力
	static const uint32_t PLAYER_HP_MAX_QUANTITY_ = 3u;
	uint32_t currentDisplayHP_ = PLAYER_HP_MAX_QUANTITY_;
	std::unique_ptr<Sprite> playerHP_[PLAYER_HP_MAX_QUANTITY_] = { nullptr };
	std::unique_ptr<Sprite> playerHPBackFrame_ = nullptr;

	//鍵
	std::unique_ptr<KeyManager> keyManager_ = {};
	uint32_t keyQuantity_ = 0u;
	bool isAbleToPickUpKey_ = false;

	std::unique_ptr<Sprite>lackOfKeyesNumberSprite_[3] = { nullptr };
	uint32_t lackOfKeyesNumber_ = 0u;


	std::unique_ptr<Sprite> toEscape_ = nullptr;

#pragma endregion

#pragma region フェード

	std::unique_ptr<Sprite> fadeSprite_ = nullptr;
	//透明度
	float fadeTransparency_ = 1.0f;

	bool isFadeIn = true;
	bool isFadeOut_ = false;

#pragma endregion


	bool isExplain_ = false;
	bool isGamePlay_ = false;


#pragma region 説明
	static const uint32_t EXPLANATION_QUANTITY_ = 2u;
	std::unique_ptr<Sprite> explanation_[EXPLANATION_QUANTITY_] = { nullptr };

	//Spaceで次に進むテキスト
	static const uint32_t SPACE_TO_NEXT_QUANTITY_ = 2u;
	std::unique_ptr<Sprite> spaceToNext_[SPACE_TO_NEXT_QUANTITY_] = { nullptr };

	uint32_t howToPlayTextureNumber_ = 0u;

#pragma endregion


#pragma region デバッグ用のオブジェクト
	Vector3 cameraTranslate = {};

	std::unique_ptr<Model> debugTower_ = nullptr;
	WorldTransform debugTowerWorldTransform_ = {};

	std::unique_ptr<Model> debugFanCollisionSphereModel_ = nullptr;
	WorldTransform debugFanCollisionSphereWorldTransform_ = {};
	Material debugFanCollisionSphereMaterial_ = {};

#pragma endregion


};