#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>

#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>

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
	//Walk
	humanModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf");
	humanAnimationModel_ = AnimationManager::GetInstance()->LoadFile("Resources/CG4/human", "walk.gltf");

	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		human_[i].reset(AnimationModel::Create(humanModelHandle));
		humanWorldTransform_[i].Initialize();
		humanAnimationTime_[i] = 0;
		humanSkeleton_[i].Create(ModelManager::GetInstance()->GetModelData(humanModelHandle).rootNode);
		humanSkinCluster_[i].Create(humanSkeleton_[i], ModelManager::GetInstance()->GetModelData(humanModelHandle));
		humanWorldTransform_[i].translate_.x = 0.0f;

	}

	humanWorldTransform_[0].translate_.y = 0.0f;
	




	//地面
	uint32_t noneModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/SilverWolf_From_Honkai_StarRail", "SilverWolf.gltf");
	noneAnimationModel_.reset(Model::Create(noneModelHandle));
	noneAnimationWorldTransform_.Initialize();
	const float SPHERE_SCALE = 0.08f;
	noneAnimationWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	noneAnimationWorldTransform_.translate_.x = 0.0f;
	noneAnimationWorldTransform_.translate_.y = 01.0f;



	camera_.Initialize();
	


}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	
	
	
#pragma region アニメーションモデル
	
	humanAnimationTime_[0] += 1.0f / 60.0f;

	
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		AnimationManager::GetInstance()->ApplyAnimation(humanSkeleton_[i], humanAnimationModel_, humanModelHandle, humanAnimationTime_[i]);
	}
	
	//現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新する
	//読み込むのは最初だけで良いと気づいた
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanSkeleton_[i].Update();
	}
	//SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanSkinCluster_[i].Update(humanSkeleton_[i]);
	}



	




	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {

		humanWorldTransform_[i].Update();
	}
#pragma endregion

	camera_.Update();
	noneAnimationWorldTransform_.Update();

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::End();

#endif


	
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
	}
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	//SimpleSkin
	//Walk
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		human_[i]->Draw(humanWorldTransform_[i], camera_, humanSkinCluster_[i]);
	}
	noneAnimationModel_->Draw(noneAnimationWorldTransform_,camera_);
	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}