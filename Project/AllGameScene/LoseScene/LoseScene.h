#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"

#include "Model.h"
#include "Camera.h"
#include <BackText.h>

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;


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
	~LoseScene();




private:

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};

	//平行光源
	DirectionalLight directionalLight_ = {};
	//マテリアル
	Material material_ = {};

	std::unique_ptr<Sprite> text_ = nullptr;
	std::unique_ptr<Sprite> title_ = nullptr;



	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;




};

