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
	modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/GLTF", "GLTFPlane.gltf",false);
	modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG4/human", "walk.gltf", false);


	//後々AnimationManagerを作ってここで読み込みたい
	//animationHandle = Animation::GetInstance()->LoadAnimationFile("Resources/CG4/AnimatedCube", "AnimatedCube.gltf");

	//SkeletonManagerも作った方がよさそう
	skeleton_ = CreateSkeleton(ModelManager::GetInstance()->GetModelData(modelHandle).rootNode);

	//Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	model_.reset(Model::Create(modelHandle));
	
	Matrix4x4 localMatrix = ModelManager::GetInstance()->GetModelData(modelHandle).rootNode.localMatrix;

	worldTransform_.Initialize(true, localMatrix);
	camera_.Initialize();

	//worldTransform_.rotate_.y = 3.1415f;
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	

	Matrix4x4 localMatrix = model_->GetAnimationLocalMatrix();
	worldTransform_.Update();
	camera_.Update();


	animationTime_ += 1.0f/60.0f;
	Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	ApplyAnimation(skeleton_, animation, animationTime_);
	//現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新する
	SkeletonUpdate(skeleton_);

	//SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
	//Update(skinCluster_,skeleton_);


#ifdef _DEBUG
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Translate", &worldTransform_.rotate_.x, -30.0f, 30.0f);
	ImGui::End();

#endif

	AdjustmentItems::GetInstance()->SaveFile(GroupName);
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	//AnimationManagerを作った方が良いかも引数を増やすの嫌だ。
	Animation animation = ModelManager::GetInstance()->GetModelAnimation(modelHandle);
	model_->Draw(worldTransform_, camera_);
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}