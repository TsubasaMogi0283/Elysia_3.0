#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "Transform.h"

#include "Particle3D.h"
#include "Audio.h"
#include <memory>
#include <Model.h>

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;


class SampleScene2 : public IGameScene {
public:
	//コンストラクタ
	SampleScene2()=default;

	

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	void PreDrawPostEffectFirst()override;

	void DrawSpriteBack()override;

	/// 描画
	void DrawObject3D()override;


	void DrawPostEffect()override;

	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite()override;

	/// デストラクタ
	~SampleScene2();



private:
	std::unique_ptr<Sprite> winTexture_ = nullptr;
	std::unique_ptr<Sprite> text_ = nullptr;

	std::unique_ptr<Sprite> black_ = nullptr;
	float transparency_ = 0.0f;
	
	//暗転している時間
	uint32_t blackOutTime_ = 0;

	//Bトリガー
	uint32_t bTriggerTime_ = 0;
	bool isBTrigger_ = false;
	bool restart_ = false;




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

