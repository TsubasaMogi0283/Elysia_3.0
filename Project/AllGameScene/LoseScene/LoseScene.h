#pragma once
/**
 * @file GameManager.h
 * @brief 負けシーンのクラス
 * @author 茂木翼
 */




#include <memory>

#include "Sprite.h"

#include "IGameScene.h"
#include "Model.h"
#include "Camera.h"
#include "BackText.h"

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

/// <summary>
/// 負けシーン
/// </summary>
class LoseScene : public IGameScene {
public:

	//コンストラクタ
	LoseScene() = default;


	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

#pragma region 描画


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
	~LoseScene();




private:

	std::unique_ptr<Sprite> failedTexture_ = nullptr;
	std::unique_ptr<Sprite> text_ = nullptr;

	std::unique_ptr<Sprite> black_ = nullptr;
	float transparency_ = 0.0f;

	//暗転している時間
	uint32_t blackOutTime_ = 0;

	//Bトリガー
	uint32_t bTriggerTime_ = 0;
	bool isBTrigger_ = false;
	bool isReturnTitle = false;




	//点滅
	bool isFlash_ = false;
	uint32_t flashTime_ = 0;
	const uint32_t FLASH_TIME_LIMIT_ = 30;
	//高速点滅
	bool isFastFlash_ = false;
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60;
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3;
	uint32_t textDisplayCount_ = 0;
	uint32_t fastFlashTime_ = 0;

};

