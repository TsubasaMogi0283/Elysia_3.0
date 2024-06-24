#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Audio.h"

#include <memory>
#include <Particle3D.h>
#include <Audio.h>
#include "BackText.h"
#include "LuminanceBasedOutline.h"
#include "DepthBasedOutline.h"
#include "RadialBlur.h"
#include "Dissolve.h"
#include "RandomEffect.h"
#include "SkinCluster.h"
#include "../../../Ellysia/Polygon/3D/SkyBox/SkyBox.h"
#include "Material.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <GrayScale.h>
#include <SepiaScale.h>
#include <Vignette.h>
#include <BoxFilter.h>
#include <GaussianFilter.h>

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
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// デストラクタ
	~SampleScene();



private:


	std::unique_ptr<Model> model_ = nullptr;
	uint32_t modelHandle = 0;
	WorldTransform worldTransform_ = {};
	
	Camera camera_ = {};

	std::unique_ptr<Sprite> sprite_ = nullptr;


	//歩き
	static const int WALK_HUMAN_AMOUNT_ = 1;
	std::unique_ptr<AnimationModel> human_[WALK_HUMAN_AMOUNT_] = { nullptr };
	uint32_t humanModelHandle = {};
	uint32_t humanAnimationModel_ = {};
	WorldTransform humanWorldTransform_[WALK_HUMAN_AMOUNT_] = {};
	Skeleton humanSkeleton_[WALK_HUMAN_AMOUNT_] = {};
	float humanAnimationTime_[WALK_HUMAN_AMOUNT_] = {};
	SkinCluster humanSkinCluster_[WALK_HUMAN_AMOUNT_] = {};

	std::unique_ptr<Model> noneAnimationModel_ = nullptr;
	WorldTransform noneAnimationWorldTransform_ = {};

	std::unique_ptr<SkyBox> skyBox_ = nullptr;
	WorldTransform skyBoxWorldTransform_ = {};

	DirectionalLight directionalLight_ = {};
	PointLight pointLight_ = {};
	SpotLight spotLight_ = {};

	Material humanMaterial_ = {};
	Material sphereMaterial = {};

	const char* GroupName = "Player";
	Vector3 position = {};


	std::unique_ptr<Audio> audio_ = nullptr;


	std::unique_ptr<BackText> back_ = nullptr;
	
	std::unique_ptr<GrayScale> grayScale_ = nullptr;
	std::unique_ptr<SepiaScale> sepiaScale_ = nullptr;
	std::unique_ptr<Vignette> vignette_ = nullptr;
	std::unique_ptr<BoxFilter> boxFilter_ = nullptr;
	std::unique_ptr<GaussianFilter> gaussianFilter_ = nullptr;

	std::unique_ptr<LuminanceBasedOutline> outLine_ = nullptr;
	std::unique_ptr<DepthBasedOutline> depthBasedOutline_ = nullptr;
	std::unique_ptr<RadialBlur> radialBlur_ = nullptr;
	std::unique_ptr<Dissolve> dissolve_ = nullptr;
	std::unique_ptr<RandomEffect> randomEffect_ = nullptr;
};