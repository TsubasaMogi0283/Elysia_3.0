#pragma once
/**
 * @file TitleScene.h
 * @brief タイトルクラス
 * @author 茂木翼
 */

#include <memory>

#include "IGameScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Camera.h"
#include "BackText.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

#pragma region 前方宣言

/// <summary>
/// ゲーム管理クラス
/// </summary>
class GameManager;

/// <summary>
/// テクスチャ管理クラス
/// </summary>
class TextureManager;

/// <summary>
/// 入力
/// </summary>
class Input;

/// <summary>
/// レベル管理クラス
/// </summary>
class LevelDataManager;

#pragma endregion


/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public IGameScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
	void Update(GameManager* gameManager)override;



	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffectFirst()override;
	
	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene()=default;


private:
	//テクスチャ管理クラス
	TextureManager* textureManager_ = nullptr;

	//入力クラス
	Input* input_ = nullptr;

	//レベルエディタ
	LevelDataManager* levelDataManager_ = nullptr;
	uint32_t levelHandle_ = 0u;

private:

	//カメラ
	Camera camera_ = {};
	//座標
	Vector3 cameraPosition_ = {};

	//平行光源
	DirectionalLight directionalLight_ = {};

	//スポットライト
	SpotLight spotLight = {};
	//マテリアル
	Material material_ = {};

	//テキスト
	std::unique_ptr<Sprite> text_ = nullptr;
	
	//背景
	std::unique_ptr<Sprite> backGround_ = nullptr;



	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> backText_ = nullptr;

	//Bボタンのトリガー
	//時間
	uint32_t bTriggerTime_ = 0;
	//フラグ
	bool isBTrigger_ = false;

	//点滅
	bool isFlash_ = false;
	//時間
	uint32_t flashTime_ = 0;
	const uint32_t FLASH_TIME_LIMIT_ = 30;
	//高速点滅
	bool isFastFlash_ = false;
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60;
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3;
	uint32_t fastFlashTime_ = 0;

	//見せる番号
	uint32_t textDisplayCount_ = 0;
	
	//スタート
	bool isStart_ = false;
};

