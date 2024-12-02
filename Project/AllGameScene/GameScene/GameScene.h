#pragma once

/**
 * @file GameManager.h
 * @brief ゲームシーンのクラス
 * @author 茂木翼
 */
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


#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Collider/CollisionManager.h"
#include "Key/Key.h"
#include "Stage/Ground/Ground.h"
#include "Key/KeyManager.h"
#include "Light/FlashLight/FlashLight.h"
#include "Enemy/EnemyManager.h"
#include "Stage/Gate/Gate.h"
#include "Fan.h"
#include "CollisionCalculation.h"
#include "Stage/Skydome/Skydome.h"

#include "Input.h"
#include "Stage/ObjectManager/ObjectManager.h"
#include "Vignette.h"

//StatePatternを使う時は必ず前方宣言をするように
//ゲームマネージャー
class GameManager;


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IGameScene {
public:

	//コンストラクタ
	GameScene();

	
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
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画前
	/// </summary>
	void PreDrawPostEffectFirst()override;

	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene()=default;



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

	/// <summary>
	/// 逃げ状態
	/// </summary>
	void EscapeCondition();

private:
	/// <summary>
	/// ゲームシーンの場面
	/// </summary>
	enum GameCondition {
		GameFadeIn,
		Explanation,
		GamePlay,
		GameFadeOut,
	};

	//初期はフェードイン
	GameCondition gameCondition_ = GameCondition::GameFadeIn;


private:
	//インプット
	Input* input_=nullptr;




private:


	//操作は全部ゲームシーンで統一させたい
	//コマンドパターンですっきりさせても良さそう

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};
	Vector3 CAMERA_POSITION_OFFSET = { 0.0f,1.0f,0.0f };

	//3人称視点
	Vector3 cameraThirdPersonViewOfPointPosition_ = {};
	Vector3 thirdPersonViewOfPointRotate_ = {};

	//回転キーXY
	bool isRotateYKey_ = false;
	bool isRotateXKey_ = false;

	//時間変化
	const float DELTA_TIME = 1.0f / 60.0f;

	//マテリアル
	Material material_ = {};

	//ポストエフェクト
	//基本
	std::unique_ptr<BackText> back_ = nullptr;
	//ビネット
	std::unique_ptr<Vignette> vignette_ = nullptr;
	const float MAX_VIGNETTE_POW_ = 1.6f;
	float vignettePow_ = 17.0f;
	float vignetteChangeTime_ = 0.0f;
	float warningTime_ = 0.0f;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

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
	std::unique_ptr<ObjectManager> objectManager_ = nullptr;


#pragma region レベルエディタに引っ越します

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	bool isEscape_ = false;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

#pragma endregion
	
	//敵管理
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	//敵のモデルハンドル
	uint32_t enemyModelHandle_ = 0u;

	//角度
	float theta_ = 0.0f;
	float originPhi_ = 0.0f;

	//コリジョン管理
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
	//鍵を取得できるかどうか
	bool isAbleToPickUpKey_ = false;


	//脱出
	std::unique_ptr<Sprite> toEscape_ = nullptr;

#pragma endregion

#pragma region フェード
	//白フェード
	std::unique_ptr<Sprite> whiteFade_ = nullptr;
	//透明度
	float whiteFadeTransparency_ = 1.0f;
	//イン
	bool isWhiteFadeIn = true;
	//アウト
	bool isWhiteFadeOut_ = false;



	//黒フェード
	std::unique_ptr<Sprite> blackFade_ = nullptr;
	//透明度
	float blackFadeTransparency_ = 0.0f;
	//イン
	bool isBlackFadeIn = false;
	//アウト
	bool isBlackFadeOut_ = false;

	//負けシーンに遷移するときの値
	const float CHANGE_TO_LOSE_SCENE_VALUE_ = 2.0f;

	
#pragma endregion

	//場面
	//説明
	bool isExplain_ = false;
	//ゲーム
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
	//カメラの位置
	Vector3 cameraTranslate = {};

	
#pragma endregion

private:
	//フェードアウトの具合
	const float FADE_OUT_INTERVAL = 0.01f;
};