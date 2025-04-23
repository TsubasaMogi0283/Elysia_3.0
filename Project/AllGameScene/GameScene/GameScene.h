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

#include "VignettePostEffect.h"
#include "Vignette.h"
#include "BaseGameScene/BaseGameScene.h"


 /// <summary>
 /// ElysiaEngine(前方宣言)
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
	void PreDrawPostEffect()override;

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

public:
	/// <summary>
	/// 細かいシーンの遷移
	/// </summary>
	/// <param name="detailGameScene">細かいシーン</param>
	void ChangeDetailScene(std::unique_ptr<BaseGameScene>detailGameScene);

	/// <summary>
	/// 処理終了
	/// </summary>
	void SetIsEnd() {
		this->isEnd_ = true;
	}

	/// <summary>
	/// 勝利の設定
	/// </summary>
	void SetIsWin() {
		this->isWin_ = true;
	}

	/// <summary>
	/// 敗北の設定
	/// </summary>
	void SetIsLose() {
		this->isLose_ = true;
	}

private:


	/// <summary>
	/// 逃げ状態
	/// </summary>
	void EscapeCondition();

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
	Elysia::TextureManager* textureManager_ = nullptr;
	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelHandle_ = 0u;
	//グローバル変数クラス
	Elysia::GlobalVariables* globalVariables_ = nullptr;



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

private:
	//カメラ
	Camera camera_ = {};
	
	//ポストエフェクト
	//ビネット
	Vignette vignette_ = {};
	std::unique_ptr<Elysia::VignettePostEffect> vignettePostEffect_ = nullptr;
	const float_t MAX_VIGNETTE_POW_ = 1.6f;
	float_t vignetteChangeTime_ = 0.0f;
	float_t warningTime_ = 0.0f;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	bool isReleaseAttack_ = false;
	//角度
	float_t theta_ = 0.0f;
	float_t phi_ = 0.0f;

	//敵管理
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	//強い敵と接触したかどうか
	bool isTouchStrongEnemy_ = false;

#pragma region ゲーム中のUI
	//UIを表示するかどうか
	bool isDisplayUI_ = false;

	//鍵取得のスプライト
	std::unique_ptr<Elysia::Sprite> pickUpKey_ = nullptr;
	//鍵管理クラス
	std::unique_ptr<KeyManager> keyManager_ = nullptr;

#pragma endregion

#pragma region フェード
	//白フェードのスプライト
	std::unique_ptr<Elysia::Sprite> whiteFadeSprite_ = nullptr;
	//透明度
	float_t fadeTransparency_ = 1.0f;

	//黒フェードのスプライト
	std::unique_ptr<Elysia::Sprite> blackFadeSprite_ = nullptr;
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



	//宝箱が開いているかどうか
	bool isOpenTreasureBox_ = false;
	std::unique_ptr<Elysia::Sprite>openTreasureBoxSprite_ = nullptr;

	Vector3 translate_ = {};
	//門の回転
	float_t rightGateRotateTheta_ = 0.0f;
	float_t leftGateRotateTheta_ = 0.0f;

	//細かいシーン
	std::unique_ptr<BaseGameScene>detailGameScene_ = nullptr;

	//処理が終了したかどうか
	bool isEnd_ = false;
	//結果
	bool isWin_ = false;
	bool isLose_ = false;

};