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
	animationHande_ = AnimationManager::GetInstance()->LoadFile("Resources/CG4/simpleSkin", "simpleSkin.gltf");
	skeleton_.Create(ModelManager::GetInstance()->GetModelData(modelHandle).rootNode);
	skinCluster_.Create(skeleton_, ModelManager::GetInstance()->GetModelData(modelHandle));

	simpleModel_.reset(AnimationModel::Create(modelHandle));
	worldTransform_.Initialize();



	

	//sneakWalk
	//Walk
	humanModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf");
	humanAnimationModel_ = AnimationManager::GetInstance()->LoadFile("Resources/CG4/human", "walk.gltf");

	human_.reset(AnimationModel::Create(humanModelHandle));
	humanWorldTransform_.Initialize();
	humanWorldTransform_.translate_.x = 2.0f;


	humanSkeleton_.Create(ModelManager::GetInstance()->GetModelData(humanModelHandle).rootNode);
	humanAnimationTime_ = 0;
	humanSkinCluster_.Create(humanSkeleton_, ModelManager::GetInstance()->GetModelData(humanModelHandle));






	//Animation無し
	uint32_t noneModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	noneAnimationModel_.reset(Model::Create(noneModelHandle));
	noneAnimationWorldTransform_.Initialize();
	noneAnimationWorldTransform_.translate_.x = -2.0f;



	camera_.Initialize();

	worldTransform_.rotate_.y = 3.1415f;
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	
	
	camera_.Update();


	animationTime_ += 1.0f/60.0f;
	humanAnimationTime_ += 1.0f / 60.0f;

	AnimationManager::GetInstance()->ApplyAnimation(skeleton_, animationHande_, modelHandle, animationTime_);
	AnimationManager::GetInstance()->ApplyAnimation(humanSkeleton_, humanAnimationModel_, humanModelHandle, humanAnimationTime_);




	//現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新する
	//読み込むのは最初だけで良いと気づいた
	skeleton_.Update();
	humanSkeleton_.Update();
	//SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
	
	skinCluster_.Update(skeleton_);
	humanSkinCluster_.Update(humanSkeleton_);

#ifdef _DEBUG
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_.rotate_.x, -30.0f, 30.0f);
	ImGui::SliderFloat("AnimationTime", &animationTime_,0.0f,10.0f);
	ImGui::End();


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::End();

#endif
	noneAnimationWorldTransform_.Update();
	worldTransform_.Update();
	humanWorldTransform_.Update();
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
	}
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	//SimpleSkin
	simpleModel_->Draw(worldTransform_, camera_, skinCluster_);

	//Walk
	human_->Draw(humanWorldTransform_, camera_, humanSkinCluster_);

	noneAnimationModel_->Draw(noneAnimationWorldTransform_,camera_);
	

}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}