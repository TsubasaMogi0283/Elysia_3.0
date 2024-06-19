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
	
	//プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();
	//初期は1人称視点
	viewOfPoint_ = FirstPerson;


	//地面
	uint32_t groundModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Ground", "Ground.obj");
	ground_.reset(Model::Create(groundModelHandle));
	groundWorldTransform_.Initialize();
	const float SPHERE_SCALE = 40.0f;
	groundWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	groundWorldTransform_.translate_.x = 0.0f;
	groundWorldTransform_.translate_.y = 0.0f;


#pragma region 敵
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD2_Enemy","TD2_Enemy.obj");
	
	//まず一個出す
	Enemy* enemy = new Enemy();
	Vector3 position = {4.0f,1.0f,0.0f};
	enemy->Initialize(enemyModelHandle_, position, {0.0f,0.0f,0.0f});
	enemys_.push_back(enemy);

	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { -2.0f,1.0f,0.0f };
	enemy2->Initialize(enemyModelHandle_, position2, {0.0f,0.0f,0.0f});
	enemys_.push_back(enemy2);



#pragma endregion





	//カメラ
	camera_.Initialize();
	camera_.translate_.y = 1.0f;
	camera_.translate_.z = -15.0f;
	cameraPosition_ = camera_ .translate_;

	CAMERA_POSITION_OFFSET = {0.0f,1.0f,1.0f};

	thirdPersonViewOfPointRotate_ = { 0.6f,0.0f,0.0f };
	cameraThirdPersonViewOfPointPosition_ = { 0.0f,25.0f,-35.0f };
	//マジでSetterでわざわざやるの面倒！
	//直したい
	theta = std::numbers::pi_v<float> / 2.0f;
	lightPosition = camera_.translate_;

	distance_ = 32.0f;
	decay_ = 0.6f;
	fallOff_ = 6.1f;
	cosAngle_ = 0.98f;
	intencity_ = 200.0f;


	//プレイヤーのライト
	uint32_t weaponLightModel= ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere","Sphere.obj");
	lightCollision_ =new LightWeapon();
	lightCollision_->Initialize(weaponLightModel);


	collisionManager_ = std::make_unique<CollisionManager>();


}



void SampleScene::GenarateEnemy() {
	Enemy* enemy = new Enemy();
	Vector3 position = {};
	Vector3 speed = {};
	enemy->Initialize(enemyModelHandle_,position,speed);

	enemys_.push_back(enemy);

}

void SampleScene::CheckCollision(std::list<Enemy*>& enemies){
	//敵
	for (auto it1 = enemies.begin(); it1 != enemies.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != enemies.end(); ++it2) {

			Vector3 diff = Subtract((*it1)->GetWorldPosition() , (*it2)->GetWorldPosition());
			float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
			float minDistance = (*it1)->GetRadius() + (*it2)->GetRadius();


#ifdef _DEBUG
			ImGui::Begin("EnemyCollision");
			ImGui::InputFloat("Distance", &distance);
			ImGui::InputFloat("Radius", &minDistance);

			ImGui::End();

#endif // DEBUG

			

			if (distance < minDistance) {
				(*it2)->SetTranslate(Add((*it2)->GetWorldPosition(), {-1.0,0.0f,0.0f}));
			}

			//if (distance < minDistance) {
			//	Vector3 correction = {};
			//	correction.x=diff.x * (minDistance - distance);
			//	correction.y = diff.y * (minDistance - distance);
			//	correction.z = diff.z * (minDistance - distance);
			//	


			//	(*it1)->SetTranslate(Add((*it1)->GetWorldPosition(), { correction.x * 0.5f,correction.y * 0.5f,correction.z * 0.5f }));
			//	(*it2)->SetTranslate(Add((*it2)->GetWorldPosition(), { correction.x * 0.5f,correction.y * 0.5f,correction.z * 0.5f }));

			//}
		}


	}
}



/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	gameManager;

	//フレーム初めに
	//コリジョンリストのクリア
	collisionManager_->ClearList();


#ifdef _DEBUG
	//Gキーで出す
	if (Input::GetInstance()->IsTriggerKey(DIK_G) == true) {
		GenarateEnemy();
	}

#endif
	
	
	
	lightDirection_.x = std::cosf(theta);
	lightDirection_.z = std::sinf(theta);
	lightPosition = player_->GetWorldPosition();
	



	//輝度
	player_->SetSpotLightIntensity(intencity_);

	//方向
	player_->SetSpotLightDirection(lightDirection_);

	//届く距離
	player_->SetSpotLightDistance(distance_);

	//減衰率
	player_->SetSpotLightDecay(decay_);
	//Falloffの開始の角度の設定
	player_->SetCosFalloffStart(fallOff_);
	//余弦
	player_->SetSpotLightCosAngle(cosAngle_);



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
		enemy->SetSpotLightPosition(lightPosition);
		//輝度
		enemy->SetSpotLightIntensity(intencity_);

		//方向
		enemy->SetSpotLightDirection(lightDirection_);

		//届く距離
		enemy->SetSpotLightDistance(distance_);

		//減衰率
		enemy->SetSpotLightDecay(decay_);
		//Falloffの開始の角度の設定
		enemy->SetCosFalloffStart(fallOff_);
		//余弦
		enemy->SetSpotLightCosAngle(cosAngle_);

	}

	collisionManager_->RegisterList(lightCollision_);

	for (auto it = enemys_.begin(); it != enemys_.end();) {
		Enemy* enemy = *it;
		if (enemy != nullptr) {
			collisionManager_->RegisterList(enemy);
		}
		it++;
	}



	if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
		viewOfPoint_ = FirstPerson;
	}
	else if (Input::GetInstance()->IsTriggerKey(DIK_3) == true) {
		viewOfPoint_ = ThirdPersonBack;
	}


	if (viewOfPoint_ == FirstPerson) {

		//カメラ
		#ifdef _DEBUG
			ImGui::Begin("Camera");
			ImGui::SliderFloat3("Pos", &CAMERA_POSITION_OFFSET.x, -30.0f, 30.0f);
			ImGui::End();
		#endif // _DEBUG

		camera_.rotate_ = { 0.0f,0.0f,0.0f };
		camera_.translate_ = Add(player_->GetWorldPosition(), CAMERA_POSITION_OFFSET);

	}
	else if (viewOfPoint_ == ThirdPersonBack) {

		#ifdef _DEBUG
			ImGui::Begin("Camera");
			ImGui::SliderFloat3("Pos", &cameraThirdPersonViewOfPointPosition_.x, -70.0f, 40.0f);
			ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);

			ImGui::End();
		#endif // _DEBUG

		camera_.rotate_ = thirdPersonViewOfPointRotate_;
		camera_.translate_ = Add(player_->GetWorldPosition(), cameraThirdPersonViewOfPointPosition_);
	}



	camera_.Update();
	
	

	//敵
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	//敵同士
	CheckCollision(enemys_);


	//プレイヤーの更新
	player_->Update();

	//地面
	groundWorldTransform_.Update();
	//ライト
	lightCollision_->Update(player_->GetWorldPosition());
	

	//当たり判定
	collisionManager_->CheckAllCollision();



	//敵がが生存していなかったら消す
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsAlive()==false) {
			delete enemy;
			return true;
		}
		return false;
	});


#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::SliderFloat("Distance", &distance_, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &fallOff_, 0.0f, 20.0f);
	ImGui::SliderFloat("CosAngle", &cosAngle_, 0.0f, 3.0f);
	ImGui::SliderFloat("intencity_", &intencity_, 0.0f, 400.0f);
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
	//プレイヤー
	player_->Draw(camera_);

	//地面
	ground_->Draw(groundWorldTransform_,camera_);
	//敵
	for (Enemy* enemy : enemys_) {
		enemy->Draw(camera_);
	}

	lightCollision_->Draw(camera_);
	
}





/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete lightCollision_;
	//敵
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	
}