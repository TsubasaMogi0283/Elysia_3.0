#pragma once
/**
 * @file LoseScene.h
 * @brief 負けシーンのクラス
 * @author 茂木翼
 */
#include <memory>

#include "IGameScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Camera.h"
#include "BackTexture.h"

//前方宣言

/// <summary>
/// ゲーム管理クラス
/// </summary>
class GameManager;

/// <summary>
/// 負けシーン
/// </summary>
class LoseScene : public IGameScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoseScene() = default;


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
	/// ポストエフェクト描画処理前
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト描画
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LoseScene();

private:
	//間隔
	const float TRANSPARENCY_INTERVAL_ = 0.01f;

	//点滅どのくらい
	const uint32_t FLASH_TIME_LIMIT_ = 30u;


	//高速点滅どのくらい
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60u;
	//高速点滅間隔
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3u;
	//タイトルに変わる時間
	const uint32_t CHANGE_TO_TITLE_TIME_ = 60 * 2;

private:
	//失敗
	std::unique_ptr<Sprite> failedTexture_ = nullptr;
	//テキスト
	std::unique_ptr<Sprite> text_ = nullptr;
	//黒背景
	std::unique_ptr<Sprite> black_ = nullptr;
	//透明度
	float transparency_ = 0.0f;
	
	//見せる番号
	uint32_t textDisplayCount_ = 0u;

	//暗転している時間
	uint32_t blackOutTime_ = 0u;

	//Bトリガー
	uint32_t bTriggerTime_ = 0u;
	//フラグ
	bool isBTrigger_ = false;
	//タイトル
	bool isReturnTitle = false;




	//点滅
	bool isFlash_ = false;
	//時間
	uint32_t flashTime_ = 0u;
	

	//高速点滅
	bool isFastFlash_ = false;
	
	//時間
	uint32_t fastFlashTime_ = 0u;

	
	

};

