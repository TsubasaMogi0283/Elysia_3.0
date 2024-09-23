#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"

#include "Model.h"
#include "Camera.h"
#include "Material.h"
#include <BackText.h>
#include <LevelDataManager.h>

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

class LevelEditorSample : public IGameScene {
public:

	//コンストラクタ
	LevelEditorSample()=default;


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
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// デストラクタ
	~LevelEditorSample();




private:
	std::unique_ptr<BackText> back_ = nullptr;
	Camera camera_ = {};
	Material material_ = {};
	DirectionalLight directionalLight_ = {};

	LevelDataManager* levelEditor_ = nullptr;

};

