#pragma once

/**
 * @file GameScene.h
 * @brief ゲームシーンのクラス
 * @author 茂木翼
 */


#include <memory>

#include "IGameScene.h"
#include "Sprite.h"
#include "Model.h"
#include "AnimationModel.h"
#include "Camera.h"
#include "Audio.h"
#include "SkinCluster.h"
#include "Material.h"
#include "SpotLight.h"
#include "DirectionalLight.h"


#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "CollisionManager.h"
#include "Key/Key.h"
#include "Key/KeyManager.h"
#include "Light/FlashLight/FlashLight.h"
#include "Enemy/EnemyManager.h"
#include "Stage/Gate/Gate.h"
#include "Fan.h"
#include "CollisionCalculation.h"

#include "Input.h"
#include "Vignette.h"
#include "BackTexture.h"
#include "LevelDataManager.h"


/// <summary>
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Ellysia {
	/// <summary>
	/// レベルエディタ
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameManager;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// モデル管理クラス
	/// </summary>
	class ModelManager;

	/// <summary>
	/// 入力クラス
	/// </summary>
	class Input;


}


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IGameScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
	void Update(Ellysia::GameManager* gameManager)override;

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
		//フェードイン
		GameFadeIn,
		
		//説明
		Explanation,
		
		//ゲームプレイ
		GamePlay,

		//フェードアウト
		GameFadeOut,
	};

	//初期はフェードイン
	GameCondition gameCondition_ = GameCondition::GameFadeIn;


private:
	//インプット
	Ellysia::Input* input_=nullptr;
	//テクスチャ管理クラス
	Ellysia::TextureManager* texturemanager_ = nullptr;
	//モデル管理クラス
	Ellysia::ModelManager* modelManager_ = nullptr;
	//レベルエディタ
	Ellysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;


private:

	//説明テクスチャの最大数
	const uint32_t MAX_EXPLANATION_NUMBER_ = 2u;
	//時間変化
	const float DELTA_TIME_ = 1.0f / 60.0f;
	//トリガーの増える値
	const uint32_t INCREASE_VALUE = 1u;
	//フェードアウトの具合
	const float FADE_OUT_INTERVAL_ = 0.01f;
	//回転の大きさ
	const float ROTATE_INTERVAL = 0.025f;
	//反応する
	const uint32_t B_REACT_TIME_ = 1u;
	//反応しない
	const uint32_t B_NO_REACT_TIME_ = 0u;
	//コントローラーの押していない時の値
	const int32_t NO_PUSH_VALUE_ = 0u;
	//完全に透明になる値
	const float PERFECT_TRANSPARENT_ = 0.0f;

private:


	//操作は全部ゲームシーンで統一させたい
	//コマンドパターンですっきりさせても良さそう

	//カメラ
	Camera camera_ = {};
	//座標のオフセット
	Vector3 cameraPositionOffset_ = {};



	//回転キーXY
	bool isRotateYKey_ = false;
	bool isRotateXKey_ = false;

	
	//マテリアル
	Material material_ = {};

	//ポストエフェクト
	//ビネット
	std::unique_ptr<Ellysia::Vignette> vignette_ = nullptr;
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

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	bool isEscape_ = false;

	
	//敵管理
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	

	//角度
	float theta_ = 0.0f;
	float originPhi_ = 0.0f;

	//コリジョン管理
	std::unique_ptr<Ellysia::CollisionManager> collisionManager_ = nullptr;

	//強い敵と接触したかどうか
	bool isTouchStrongEnemy_ = false;
	
#pragma region ゲーム中のUI
	//UIを表示するかどうか
	bool isDisplayUI_ = false;

	//脱出テキストのスプライト
	std::unique_ptr<Sprite> escapeText_ = nullptr;

	//操作のスプライト
	std::unique_ptr<Sprite> operation_ = nullptr;

	//鍵取得のスプライト
	std::unique_ptr<Sprite> pickUpKey_ = nullptr;

	//プレイヤーの体力
	static const uint32_t PLAYER_HP_MAX_QUANTITY_ = 3u;
	//現在のHP
	uint32_t currentDisplayHP_ = PLAYER_HP_MAX_QUANTITY_;
	//プレイヤーHPのスプライト
	std::unique_ptr<Sprite> playerHP_[PLAYER_HP_MAX_QUANTITY_] = { nullptr };
	//背景フレーム
	std::unique_ptr<Sprite> playerHPBackFrame_ = nullptr;

	//鍵管理クラス
	std::unique_ptr<KeyManager> keyManager_ = {};
	//鍵の数
	uint32_t keyQuantity_ = 0u;
	//鍵を取得できるかどうか
	bool isAbleToPickUpKey_ = false;


	//脱出のスプライト
	std::unique_ptr<Sprite> toEscape_ = nullptr;

#pragma endregion

#pragma region フェード
	//白フェードのスプライト
	std::unique_ptr<Sprite> whiteFade_ = nullptr;
	//透明度
	float whiteFadeTransparency_ = 1.0f;
	//イン
	bool isWhiteFadeIn = true;
	//アウト
	bool isWhiteFadeOut_ = false;



	//黒フェードのスプライト
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
	//説明の数
	static const uint32_t EXPLANATION_QUANTITY_ = 2u;
	//説明スプライト
	std::unique_ptr<Sprite> explanation_[EXPLANATION_QUANTITY_] = { nullptr };

	//Spaceで次に進むテキスト
	//数
	static const uint32_t SPACE_TO_NEXT_QUANTITY_ = 2u;
	//次へのスプライト
	std::unique_ptr<Sprite> spaceToNext_[SPACE_TO_NEXT_QUANTITY_] = { nullptr };

	//
	uint32_t howToPlayTextureNumber_ = 0u;

#pragma endregion

	//門の回転
	float gateRotateTheta_ = 0.0f;

};