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


	//敵
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD2_Enemy","TD2_Enemy.obj");
	
	//まず一個出す
	Enemy* enemy = new Enemy();
	Vector3 position = {4.0f,1.0f,0.0f};
	enemy->Initialize(enemyModelHandle_, position, {0.0f,0.0f,0.0f});
	enemys_.push_back(enemy);

	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { -2.0f,1.0f,0.0f };
	enemy2->Initialize(enemyModelHandle_, position2, {0.010f,0.0f,0.0f});
	enemys_.push_back(enemy2);


	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	test_.reset(Sprite::Create(textureHandle, { 0.0f,0.0f }));
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	camera_.Initialize();
	camera_.translate_.y = 1.0f;
	camera_.translate_.z = -15.0f;



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



			ImGui::Begin("EnemyCollision");
			ImGui::InputFloat("Distance",&distance);
			ImGui::InputFloat("Radius", &minDistance);
			
			ImGui::End();


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

	//カメラ
	camera_.Update();
	//敵同士
	CheckCollision(enemys_);

	
	//敵
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	

	//地面
	groundWorldTransform_.Update();
	//ライト
	Vector3 cameraWorldPosition = { camera_.worldMatrix_.m[3][0],camera_.worldMatrix_.m[3][1],camera_.worldMatrix_.m[3][2] };
	lightCollision_->Update(cameraWorldPosition);
	

	//当たり判定
	collisionManager_->CheckAllCollision();




	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsAlive()==false) {
			delete enemy;
			return true;
		}
		return false;
	});

	test_->SetColor(color_);

#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::SliderFloat("Distance", &distance_, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &fallOff_, 0.0f, 20.0f);
	ImGui::SliderFloat("CosAngle", &cosAngle_, 0.0f, 3.0f);
	ImGui::SliderFloat("intencity_", &intencity_, 0.0f, 400.0f);
	ImGui::End();

	ImGui::Begin("Sprite");
	ImGui::SliderFloat4("color", &color_.x, 0.0f, 1.0f);
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

	lightCollision_->Draw(camera_);
	test_->Draw();
	
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