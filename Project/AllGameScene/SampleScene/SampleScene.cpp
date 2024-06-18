#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "SampleScene2/SampleScene2.h"

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
	//modelHandle =ModelManager::GetInstance()->LoadModelFile("Resources/CG4/AnimatedCube", "AnimatedCube.gltf",false);
	modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/terrain", "terrain.obj");
	worldTransform_.Initialize();
	model_.reset(Model::Create(modelHandle));
	model_->SetLighting(1);
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
	uint32_t noneModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	noneAnimationModel_.reset(Model::Create(noneModelHandle));
	noneAnimationWorldTransform_.Initialize();
	const float SPHERE_SCALE = 1.0f;
	noneAnimationWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	noneAnimationWorldTransform_.translate_.x = 0.0f;
	noneAnimationWorldTransform_.translate_.y = 0.0f;

	

	camera_.Initialize();
	camera_.translate_ = { 0.0f,1.0f,-10.0f };


	uint32_t skyBoxTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/CG4/SkyBox/rostock_laage_airport_4k.dds");
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Create(skyBoxTextureHandle);
	skyBoxWorldTransform_.Initialize();
	const float SKYBOX_SCALE = 20.0f;
	skyBoxWorldTransform_.scale_ = { SKYBOX_SCALE ,SKYBOX_SCALE ,SKYBOX_SCALE };

	noneAnimationModel_->SetEviromentTexture(skyBoxTextureHandle);
	//human_[0]->SetEviromentTexture(skyBoxTextureHandle);


	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("Resources/White.png");
	sprite_.reset(Sprite::Create(textureHandle, { 0.0f,0.0f }));
	sprite_->SetScale({0.5f, 0.5f});
	
	back_ = std::make_unique< BackText>();
	back_->Initialize();

	back1_= std::make_unique< BackText>();
	back1_->Initialize();
	
	//
	//radialBlur_ = std::make_unique < RadialBlur>();
	//radialBlur_->Initialize();
	//
	//outLine_ = std::make_unique < LuminanceBasedOutline>();
	//outLine_->Initialize();
	////depthBasedOutline_ = new DepthBasedOutline();
	////depthBasedOutline_->Initialize();
	//
	//
	//uint32_t dissolveTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/CG5/00/08/noise0.png");
	//dissolve_ = std::make_unique <Dissolve>();
	//dissolve_->Initialize(dissolveTextureHandle);
	//
	//randomEffect_ = std::make_unique < RandomEffect>();
	//randomEffect_->Initialize();

	

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


	sprite_->SetPosition(position);

	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanWorldTransform_[i].Update();
	}
#pragma endregion


#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -40.0f, 20.0f);
	ImGui::End();


	ImGui::Begin("Sprite");
	ImGui::SliderFloat3("Z",&position.x,0.0f,1280.0f);
	ImGui::End();
	
#endif
	const float CAMERA_MOVE_SPEED = 0.2f;
	Vector3 move = {};
	Vector3 rotateMove = {};

	//Y
	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		move.y = 1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		move.y = -1.0f;
	}
	//X
	else if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		move.x = 1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		move.x = -1.0f;
	}
	//Z
	else if (Input::GetInstance()->IsPushKey(DIK_O) == true) {
		move.z = 1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_L) == true) {
		move.z = -1.0f;
	}


	else {
		move.x = 0.0f;
		move.y = 0.0f;
		move.z = 0.0f;
	}

	//回転
	const float ROTATE_MOVE_SPEED = 0.01f;
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		rotateMove.y = -1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		rotateMove.y = 1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		rotateMove.x = -1.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		rotateMove.x = 1.0f;
	}

	else {
		rotateMove.y = 0.0f;
		rotateMove.x = 0.0f;
	}

	camera_.translate_ = Add(camera_.translate_, { move.x* CAMERA_MOVE_SPEED,move.y* CAMERA_MOVE_SPEED,move.z*CAMERA_MOVE_SPEED });
	camera_.rotate_ = Add(camera_.rotate_, { rotateMove.x * ROTATE_MOVE_SPEED,rotateMove.y * ROTATE_MOVE_SPEED,rotateMove.z * ROTATE_MOVE_SPEED });

	camera_.Update();
	worldTransform_.Update();
	noneAnimationWorldTransform_.Update();
	skyBoxWorldTransform_.Update();
#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::End();

#endif
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		AdjustmentItems::GetInstance()->SaveFile(GroupName);
		gameManager->ChangeScene(new SampleScene2());
	}


	
}

void SampleScene::DrawSpriteBack(){

	//sprite_->Draw();
}

void SampleScene::PreDrawPostEffectFirst(){
	//dissolve_->PreDraw();

	//randomEffect_->PreDraw();
	back_->PreDraw();
	back1_->PreDraw();
	//radialBlur_->PreDraw();
	//outLine_->PreDraw();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::DrawObject3D() {
	
	skyBox_->Draw(skyBoxWorldTransform_,camera_);
	//SimpleSkin
	//Walk
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		human_[i]->Draw(humanWorldTransform_[i], camera_, humanSkinCluster_[i]);
	}
	noneAnimationModel_->Draw(noneAnimationWorldTransform_,camera_);
	model_->Draw(worldTransform_, camera_);


	
}



void SampleScene::DrawPostEffect(){
	//radialBlur_->Draw();
	//dissolve_->Draw();
	

	//randomEffect_->Draw();
	back_->Draw();
	back1_->Draw();
	//radialBlur_->Draw();
	//outLine_->Draw();
}

void SampleScene::DrawSprite(){
	sprite_->Draw();
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
}