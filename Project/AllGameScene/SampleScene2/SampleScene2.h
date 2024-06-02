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
	SampleScene2();

	

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	void PreDrawPostEffectFirst()override;
	void PreDrawPostEffectSecond()override;

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
	std::unique_ptr<Sprite> sprite_ = nullptr;

	std::unique_ptr<Sprite> spriteUV_ = nullptr;

	std::unique_ptr<Sprite> howToSprite_ = nullptr;


	
};

