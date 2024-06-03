#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>

#include "ModelManager.h"

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
	//modelHandle =ModelManager::GetInstance()->LoadModelFile("Resources/Sample/GLTF", "GLTFPlane.obj",false);
	//modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/GLTF", "GLTFPlane.gltf",false);
	//modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf", true);
	//modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/simpleSkin", "simpleSkin.gltf", true);
	//
	//
	////後々AnimationManagerを作ってここで読み込みたい
	////animationHandle = Animation::GetInstance()->LoadAnimationFile("Resources/CG4/AnimatedCube", "AnimatedCube.gltf");
	//
	////SkeletonManagerも作った方がよさそう
	//skeleton_.Create(ModelManager::GetInstance()->GetModelData(modelHandle).rootNode);
	//
	//skinCluster_.Create(skeleton_, ModelManager::GetInstance()->GetModelData(modelHandle));
	////Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	//simpleModel_.reset(Model::Create(modelHandle));
	////Matrix4x4 localMatrix = ModelManager::GetInstance()->GetModelData(modelHandle).rootNode.localMatrix;
	//worldTransform_.Initialize();



	

	//sneakWalk
	//Walk
	humanModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf", true);
	human_.reset(Model::Create(humanModelHandle));
	humanWorldTransform_.Initialize();
	//humanWorldTransform_.translate_.x = 2.0f;


	Node node = ModelManager::GetInstance()->GetModelData(humanModelHandle).rootNode;
	humanSkeleton_.Create(ModelManager::GetInstance()->GetModelData(humanModelHandle).rootNode);
	humanAnimationTime_ = 0;
	humanSkinCluster_.Create(humanSkeleton_, ModelManager::GetInstance()->GetModelData(humanModelHandle));





	camera_.Initialize();

	worldTransform_.rotate_.y = 3.1415f;
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	
	
	camera_.Update();


	//animationTime_ += 1.0f/60.0f;
	humanAnimationTime_ += 1.0f / 60.0f;
	if (animationTime_ > 7.0f) {
		animationTime_ = 0.0f;
		
	}
	if (humanAnimationTime_ > 2.0f) {
		humanAnimationTime_ = 0.0f;
	}

	//Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	//ApplyAnimation(skeleton_, animation, animationTime_);

	Animation humanAnimation = ModelManager::GetInstance()->GetModelAnimation(humanModelHandle);
	ApplyAnimation(humanSkeleton_, humanAnimation, humanAnimationTime_);




	//現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新する
	//読み込むのは最初だけで良いと気づいた
	//SkeletonUpdate(skeleton_);
	//skeleton_.Update();
	humanSkeleton_.Update();
	//SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
	
	//skinCluster_.Update(skeleton_);
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

	//worldTransform_.Update();
	humanWorldTransform_.Update();
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
	}
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	//AnimationManagerを作った方が良いかも引数を増やすの嫌だ。
	//Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	//simpleModel_->Draw(worldTransform_, camera_, skinCluster_);


	human_->Draw(humanWorldTransform_, camera_, humanSkinCluster_);



}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}