#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"

#include "Model.h"
#include "Camera.h"
#include <BackText.h>
#include "DirectionalLight.h"

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

class TitleScene : public IGameScene {
public:

	//コンストラクタ
	TitleScene()=default;


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
	~TitleScene();




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


	uint32_t bTriggerTime_ = 0;
	bool isBTrigger_ = false;

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



	//スタート
	bool isStart_ = false;
};

