#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

#include <memory>
#include <Particle3D.h>
#include <Audio.h>
#include "SkinCluster.h"

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
	void Draw()override;



private:
	Camera camera_ = {};

	//SimpleSkin
	std::unique_ptr<AnimationModel> simpleModel_ = nullptr;
	uint32_t modelHandle = 0;
	uint32_t animationHande_ = 0;
	WorldTransform worldTransform_ = {};
	
	Skeleton skeleton_ = {};
	float animationTime_ = 0;
	SkinCluster skinCluster_ = {};




	//歩き
	std::unique_ptr<AnimationModel> human_ = nullptr;
	uint32_t humanModelHandle = 0;
	uint32_t humanAnimationModel_ = 0;
	WorldTransform humanWorldTransform_ = {};

	Skeleton humanSkeleton_ = {};
	float humanAnimationTime_ = 0;
	SkinCluster humanSkinCluster_ = {};



	const char* GroupName = "Player";
};