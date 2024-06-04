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




	const char* GroupName = "Player";
};