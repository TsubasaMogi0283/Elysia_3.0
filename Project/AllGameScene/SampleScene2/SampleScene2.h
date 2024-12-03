#pragma once
/**
 * @file SampleScene2.h
 * @brief サンプルクラス2
 * @author 茂木翼
 */

#include "IGameScene.h"

#include "Sprite.h"
#include "Transform.h"

#include "Particle3D.h"
#include "Audio.h"
#include <memory>
#include <Model.h>

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

/// <summary>
/// サンプルクラス2
/// </summary>
class SampleScene2 : public IGameScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SampleScene2()=default;

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
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffectFirst()override;

	/// <summary>
	/// 描画(3D描画)
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void DrawPostEffect()override;

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SampleScene2();



private:
	//勝ち
	std::unique_ptr<Sprite> winTexture_ = nullptr;
	//テキスト
	std::unique_ptr<Sprite> text_ = nullptr;

	//背景
	std::unique_ptr<Sprite> black_ = nullptr;
	//透明度
	float transparency_ = 0.0f;
	
	//暗転している時間
	uint32_t blackOutTime_ = 0u;

	//Bトリガー
	uint32_t bTriggerTime_ = 0u;
	bool isBTrigger_ = false;
	bool restart_ = false;




	//点滅
	bool isFlash_ = false;
	uint32_t flashTime_ = 0u;
	const uint32_t FLASH_TIME_LIMIT_ = 30u;
	//高速点滅
	bool isFastFlash_ = false;
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60u;
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3u;
	uint32_t textDisplayCount_ = 0u;
	uint32_t fastFlashTime_ = 0u;

};

