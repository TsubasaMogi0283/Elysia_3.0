#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>

#include "ModelManager.h"
#include "AnimationManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	
	//GLTF2.0
	//「GLTF Separate(.gltf+bin+Texture)」、「オリジナルを保持」で
	modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/simpleSkin", "simpleSkin.gltf");
	
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		skeleton_[i].Create(ModelManager::GetInstance()->GetModelData(modelHandle).rootNode);
		skinCluster_[i].Create(skeleton_[i], ModelManager::GetInstance()->GetModelData(modelHandle));
		animationHande_ = AnimationManager::GetInstance()->LoadFile("Resources/CG4/simpleSkin", "simpleSkin.gltf");

		simpleModel_[i].reset(AnimationModel::Create(modelHandle));
		worldTransform_[i].Initialize();

	}
	worldTransform_[0].translate_.x=0.0f;
	worldTransform_[0].translate_.y = 0.5f;
	worldTransform_[0].translate_.z = 0.0f;
	worldTransform_[1].translate_.y=-1.0f;
	worldTransform_[1].translate_.y = -1.0f;
	worldTransform_[1].translate_.z = -1.0f;



	

	//sneakWalk
	//Walk
	humanModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf");
	humanAnimationModel_ = AnimationManager::GetInstance()->LoadFile("Resources/CG4/human", "walk.gltf");

	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		human_[i].reset(AnimationModel::Create(humanModelHandle));
		humanWorldTransform_[i].Initialize();
		humanAnimationTime_[i] = 0;
		humanSkeleton_[i].Create(ModelManager::GetInstance()->GetModelData(humanModelHandle).rootNode);
		humanSkinCluster_[i].Create(humanSkeleton_[i], ModelManager::GetInstance()->GetModelData(humanModelHandle));
		humanWorldTransform_[i].translate_.x = 2.0f;

	}

	humanWorldTransform_[0].translate_.y = 0.5f;
	humanWorldTransform_[1].translate_.y = -2.0f;

	




	//Animation無し

	humanNoneAnimation_.reset(Model::Create(humanModelHandle));
	humanNoneAnimationWorldTransform_.Initialize();
	humanNoneAnimationWorldTransform_.translate_.x = -2.0f;
	humanNoneAnimationWorldTransform_.translate_.y = 0.5f;





	//球
	uint32_t noneModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	noneAnimationModel_.reset(Model::Create(noneModelHandle));
	noneAnimationWorldTransform_.Initialize();
	const float SPHERE_SCALE = 0.5f;
	noneAnimationWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	noneAnimationWorldTransform_.translate_.x = -2.0f;
	noneAnimationWorldTransform_.translate_.y = -1.0f;



	camera_.Initialize();
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		worldTransform_[i].rotate_.y = 3.1415f;
	}
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	
	
	camera_.Update();


	animationTime_[0] += 1.0f / 60.0f;
	animationTime_[1] += 2.0f / 60.0f;

	
	humanAnimationTime_[0] += 1.0f / 60.0f;
	humanAnimationTime_[1] += 3.0f / 60.0f;

	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		AnimationManager::GetInstance()->ApplyAnimation(skeleton_[i], animationHande_, modelHandle, animationTime_[i]);
	};
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		AnimationManager::GetInstance()->ApplyAnimation(humanSkeleton_[i], humanAnimationModel_, humanModelHandle, humanAnimationTime_[i]);
	}
	



	//現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新する
	//読み込むのは最初だけで良いと気づいた
	
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		skeleton_[i].Update();
	}
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanSkeleton_[i].Update();
	}
	
	//SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
	
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		skinCluster_[i].Update(skeleton_[i]);
	}
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanSkinCluster_[i].Update(humanSkeleton_[i]);
	}
#ifdef _DEBUG
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_[0].rotate_.x, -30.0f, 30.0f);
	ImGui::SliderFloat("AnimationTime", &animationTime_[0], 0.0f, 10.0f);
	ImGui::End();


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::End();

#endif
	humanNoneAnimationWorldTransform_.Update();

	noneAnimationWorldTransform_.Update();
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		worldTransform_[i].Update();
	}
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanWorldTransform_[i].Update();
	}
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
	}
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	//SimpleSkin
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		simpleModel_[i]->Draw(worldTransform_[i], camera_, skinCluster_[i]);
	}
	//Walk
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		human_[i]->Draw(humanWorldTransform_[i], camera_, humanSkinCluster_[i]);
	}
	noneAnimationModel_->Draw(noneAnimationWorldTransform_,camera_);
	
	humanNoneAnimation_->Draw(humanNoneAnimationWorldTransform_,camera_);
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}