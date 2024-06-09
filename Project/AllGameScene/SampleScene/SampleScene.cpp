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
	




	//地面
	uint32_t groundModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Ground", "Ground.obj");
	ground_.reset(Model::Create(groundModelHandle));
	groundWorldTransform_.Initialize();
	const float SPHERE_SCALE = 40.0f;
	groundWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	groundWorldTransform_.translate_.x = 0.0f;
	groundWorldTransform_.translate_.y = 0.0f;



	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Enemy","enemy.obj");
	
	//まず一個出す
	Enemy* enemy = new Enemy();
	Vector3 position = {4.0f,1.0f,0.0f};
	enemy->Initialize(enemyModelHandle_,position);
	enemys_.push_back(enemy);


	camera_.Initialize();
	camera_.translate_.y = 1.0f;
	camera_.translate_.z = -15.0f;

	theta = std::numbers::pi_v<float> / 2.0f;
	lightPosition = camera_.translate_;

	distance_ = 50.0f;
	decay_ = 0.0f;
	fallOff_ = 6.1f;
	cosAngle_ = 0.98f;
	intencity_ = 200.0f;
}



void SampleScene::GenarateEnemy() {
	Enemy* enemy = new Enemy();
	Vector3 position = {};
	enemy->Initialize(enemyModelHandle_,position);

	enemys_.push_back(enemy);

}



/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	//1キーで出す
	if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
		GenarateEnemy();
	}

	
	
	const float MOVE_SPEED = 0.1f ;
	
	Vector3 move = {};
	
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		move.x = 1.0f;
		//theta = 0.0f;
		//camera_.rotate_.y = std::numbers::pi_v<float> / 2.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		move.x = -1.0f;
		//theta = std::numbers::pi_v<float>;
		
		//camera_.rotate_.y = -std::numbers::pi_v<float> / 2.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		move.z = 1.0f;
		
		//camera_.rotate_.y = 0.0f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		move.z = -1.0f;
		//theta = std::numbers::pi_v<float>*3/2.0f;
		//camera_.rotate_.y = std::numbers::pi_v<float>;
	}
	
	lightDirection_.x = std::cosf(theta);
	lightDirection_.z = std::sinf(theta);


	camera_.translate_ = Add(camera_.translate_, { move.x * MOVE_SPEED,move.y * MOVE_SPEED,move.z * MOVE_SPEED });

	lightPosition = camera_.translate_;	
	lightPosition = Add(lightPosition, { move.x* MOVE_SPEED,move.y* MOVE_SPEED,move.z* MOVE_SPEED });

	

	ground_->SetSpotLightPosition(lightPosition);
	//輝度
	ground_->SetSpotLightIntensity(intencity_);

	//方向
	ground_->SetSpotLightDirection(lightDirection_);


	//届く距離
	ground_->SetSpotLightDistance(distance_);

	//減衰率
	ground_->SetSpotLightDecay(decay_);
	//Falloffの開始の角度の設定
	ground_->SetCosFalloffStart(fallOff_);
	//余弦
	ground_->SetSpotLightCosAngle(cosAngle_);

	//敵
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	//地面
	groundWorldTransform_.Update();
	//カメラ
	camera_.Update();




#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::SliderFloat("Distance", &distance_, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &fallOff_, 0.0f, 20.0f);
	ImGui::SliderFloat("CosAngle", &cosAngle_, 0.0f, 3.0f);
	ImGui::SliderFloat("intencity_", &intencity_, 0.0f, 400.0f);
	ImGui::End();



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
	//地面
	ground_->Draw(groundWorldTransform_,camera_);
	//敵
	for (Enemy* enemy : enemys_) {
		enemy->Draw(camera_);
	}
	
}





/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}