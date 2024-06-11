#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

#include <memory>
#include <Particle3D.h>
#include <Audio.h>
#include "../../BackTest/BackText.h"
#include "../../Project/OutLine/LuminanceBasedOutline.h"

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	
	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

#pragma region 描画

	/// <summary>
	/// ポストエフェクト掛ける前のスプライト
	/// </summary>
	void DrawSpriteBack()override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;


	void PreDrawPostEffectFirst()override;
	void PreDrawPostEffectSecond()override;
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// デストラクタ
	~SampleScene();



private:

	Audio* audio_ = nullptr;
	uint32_t audioHandle_ = 0u;

	float_t pan_ = 0.0f;
	int pitch_ = 0;
	float cutOff_ = 1.0f;

	std::unique_ptr<Model> model_ = nullptr;
	uint32_t modelHandle = 0;
	WorldTransform worldTransform_ = {};
	Camera camera_ = {};

	std::unique_ptr<Sprite> sprite_ = nullptr;


	const char* GroupName = "Player";
	Vector3 position = {};

	BackText* back_ = nullptr;

	LuminanceBasedOutline* outLine_ = nullptr;

};