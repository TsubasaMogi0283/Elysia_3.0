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


//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void DrawObject3D()override;

	void DrawSprite()override;

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

	const char* GroupName = "Player";
};