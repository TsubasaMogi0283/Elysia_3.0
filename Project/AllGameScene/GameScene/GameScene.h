#pragma once

/**
 * @file GameScene.h
 * @brief ゲームシーンのクラス
 * @author 茂木翼
 */

#include <memory>

#include "IGameScene.h"
#include "Sprite.h"
#include "Camera.h"
#include "SpotLight.h"

#include "Player/Player.h"
#include "CollisionManager.h"
#include "Key/KeyManager.h"
#include "Enemy/EnemyManager.h"
#include "Stage/Gate/Gate.h"
#include "CollisionCalculation.h"

#include "Vignette.h"



 /// <summary>
 /// EllysiaEngine(前方宣言)
 /// </summary>
namespace Elysia {
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

	/// <summary>
	/// 調整項目クラス
	/// </summary>
	class GlobalVariables;
}


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Elysia::IGameScene {
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
	void Update(Elysia::GameManager* gameManager)override;

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
	~GameScene() = default;

private:



	/// <summary>
	/// ステージオブジェクトとの当たり判定
	/// </summary>
	void ObjectCollision();

	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	void PlayerMove();

	/// <summary>
	/// プレイヤーの回転
	/// </summary>
	void PlayerRotate();

	/// <summary>
	/// 逃げ状態
	/// </summary>
	void EscapeCondition();

	/// <summary>
	/// コリジョン管理クラスに登録
	/// </summary>
	void RegisterToCollisionManager();

	/// <summary>
	/// ビネットの処理
	/// </summary>
	void VigntteProcess();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();


private:
	//インプット
	Elysia::Input* input_ = nullptr;
	//テクスチャ管理クラス
	Elysia::TextureManager* texturemanager_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;
	//グローバル変数クラス
	Elysia::GlobalVariables* globalVariables_ = nullptr;

private:
	//カメラ
	const std::string GAME_SCENE_CAMERA_NAME_ = "GameSceneCamera";
	//ディゾルブ
	const std::string HEIGHT_OFFSET_ = "HeightOffset";


private:

	//説明テクスチャの最大数
	const uint32_t MAX_EXPLANATION_NUMBER_ = 2u;
	//時間変化
	const float_t DELTA_TIME_ = 1.0f / 60.0f;
	//フェードアウトの具合
	const float_t FADE_OUT_INTERVAL_ = 0.01f;
	//負けシーンに遷移するときの値
	const float_t CHANGE_TO_LOSE_SCENE_VALUE_ = 2.0f;

	//完全に透明になる値
	const float_t PERFECT_TRANSPARENT_ = 0.0f;

	//ライトの幅
	//最大
	const float_t LIGHT_MAX_RANGE_ = 0.35f;
	//最小
	const float_t LIGHT_MIN_RANGE_ = 0.08f;

private:
	//カメラ
	Camera camera_ = {};
	//座標のオフセット
	Vector3 cameraPositionOffset_ = {};

	
	//ポストエフェクト
	//ビネット
	std::unique_ptr<Elysia::Vignette> vignette_ = nullptr;
	const float_t MAX_VIGNETTE_POW_ = 1.6f;
	float_t vignettePow_ = 17.0f;
	float_t vignetteChangeTime_ = 0.0f;
	float_t warningTime_ = 0.0f;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	bool isReleaseAttack_ = false;
	//角度
	float_t theta_ = 0.0f;
	float_t originPhi_ = 0.0f;

	//敵管理
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	//強い敵と接触したかどうか
	bool isTouchStrongEnemy_ = false;

	//コリジョン管理
	std::unique_ptr<Elysia::CollisionManager> collisionManager_ = nullptr;

	
#pragma region ゲーム中のUI
	//UIを表示するかどうか
	bool isDisplayUI_ = false;

	//脱出テキストのスプライト
	std::unique_ptr<Elysia::Sprite> escapeText_ = nullptr;

	//操作のスプライト
	std::unique_ptr<Elysia::Sprite> operation_ = nullptr;

	//鍵取得のスプライト
	std::unique_ptr<Elysia::Sprite> pickUpKey_ = nullptr;

	//プレイヤーの体力
	static const uint32_t PLAYER_HP_MAX_QUANTITY_ = 3u;
	//現在のHP
	uint32_t currentDisplayHP_ = PLAYER_HP_MAX_QUANTITY_;
	//プレイヤーHPのスプライト
	std::unique_ptr<Elysia::Sprite> playerHP_[PLAYER_HP_MAX_QUANTITY_] = { nullptr };
	//背景フレーム
	std::unique_ptr<Elysia::Sprite> playerHPBackFrame_ = nullptr;

	//鍵管理クラス
	std::unique_ptr<KeyManager> keyManager_ = {};



	//脱出のスプライト
	std::unique_ptr<Elysia::Sprite> toEscape_ = nullptr;

#pragma endregion

#pragma region フェード
	//白フェードのスプライト
	std::unique_ptr<Elysia::Sprite> whiteFade_ = nullptr;
	//透明度
	float_t whiteFadeTransparency_ = 1.0f;
	//イン
	bool isWhiteFadeIn = true;
	//アウト
	bool isWhiteFadeOut_ = false;



	//黒フェードのスプライト
	std::unique_ptr<Elysia::Sprite> blackFade_ = nullptr;
	//透明度
	float_t blackFadeTransparency_ = 0.0f;
	//イン
	bool isBlackFadeIn = false;
	//アウト
	bool isBlackFadeOut_ = false;

	

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
	std::array<std::unique_ptr<Elysia::Sprite>, EXPLANATION_QUANTITY_> explanation_ = { nullptr };

	//Spaceで次に進むテキスト
	std::array<std::unique_ptr<Elysia::Sprite>, EXPLANATION_QUANTITY_> spaceToNext_ = { nullptr };

	//テクスチャの番号
	uint32_t howToPlayTextureNumber_ = 0u;

#pragma endregion

	//ゲート
	std::unique_ptr<Gate> gate_ = nullptr;
	bool isEscape_ = false;

	//宝箱が開いているかどうか
	bool isOpenTreasureBox_ = false;
	std::unique_ptr<Elysia::Sprite>openTreasureBoxSprite_ = nullptr;

	Vector3 translate_ = {};
	//門の回転
	float_t rightGateRotateTheta_ = 0.0f;
	float_t leftGateRotateTheta_ = 0.0f;

};