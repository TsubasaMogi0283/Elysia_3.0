#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "SampleScene2/SampleScene2.h"

#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>
#include <TextureManager.h>

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

#pragma region 鍵
	Key* key1 = new Key();
	uint32_t keyModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","Cube.obj");
	Vector3 keyPosition = { -5.0f,1.0f,1.0f };
	key1->Initialize(keyModelHandle,keyPosition );
	keyes_.push_back(key1);

#pragma endregion


#pragma region 敵
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD2_Enemy", "TD2_Enemy.obj");

	//まず一個出す
	Enemy* enemy = new Enemy();
	enemy->Initialize(enemyModelHandle_, { 4.0f,1.0f,0.0f }, { 0.0f,0.0f,0.0f });
	enemys_.push_back(enemy);

	Enemy* enemy2 = new Enemy();
	enemy2->Initialize(enemyModelHandle_, { -2.0f,1.0f,0.0f }, { 0.0f,0.0f,0.0f });
	enemys_.push_back(enemy2);



#pragma endregion





	//カメラ
	camera_.Initialize();
	camera_.translate_.y = 1.0f;
	camera_.translate_.z = -15.0f;
	camera_.rotate_.y = std::numbers::pi_v<float> / 2.0f;
	cameraPosition_ = camera_.translate_;

	CAMERA_POSITION_OFFSET = { 0.0f,1.0f,1.0f };

	thirdPersonViewOfPointRotate_ = { 0.6f,0.0f,0.0f };
	cameraThirdPersonViewOfPointPosition_ = { 0.0f,25.0f,-35.0f };

	firstPersonRotate_ = 0.0f;


	theta = std::numbers::pi_v<float> / 2.0f;
	lightPosition = camera_.translate_;

	distance_ = 32.0f;
	decay_ = 0.6f;
	fallOff_ = 6.1f;
	cosAngle_ = 0.98f;
	intencity_ = 200.0f;


	//プレイヤーのライト
	uint32_t weaponLightModel = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	lightCollision_ = new LightWeapon();
	lightCollision_->Initialize(weaponLightModel);


	collisionManager_ = std::make_unique<CollisionManager>();




	back_ = std::make_unique< BackText>();
	back_->Initialize();

	material_.Initialize();
	material_.lightingKinds_ = Spot;
	//material_.lightingKinds_ = Directional;

	spotLight_.Initialize();
	directionalLight_.Initialize();
}



void SampleScene::GenarateEnemy() {
	Enemy* enemy = new Enemy();
	Vector3 speed = {};
	enemy->Initialize(enemyModelHandle_, {0.0f,0.0f,0.0f}, speed);

	enemys_.push_back(enemy);

}

void SampleScene::CheckCollision(std::list<Enemy*>& enemies) {
	//敵
	for (auto it1 = enemies.begin(); it1 != enemies.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != enemies.end(); ++it2) {

			Vector3 diff = Subtract((*it1)->GetWorldPosition(), (*it2)->GetWorldPosition());
			float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
			float minDistance = (*it1)->GetRadius() + (*it2)->GetRadius();


#ifdef _DEBUG
			ImGui::Begin("EnemyCollision");
			ImGui::InputFloat("Distance", &distance);
			ImGui::InputFloat("Radius", &minDistance);

			ImGui::End();

#endif // DEBUG



			if (distance < minDistance) {
				(*it2)->SetTranslate(Add((*it2)->GetWorldPosition(), { -1.0,0.0f,0.0f }));
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

void SampleScene::KeyCollision(){

	//リストにする必要性が感じられないので後で配列に直すかも
	//鍵
	for (Key* key : keyes_) {


		if (key->GetIsPickUp() == false) {
			//判定は円で良いかも
			Vector3 distance = {};
			distance.x = std::powf((player_->GetWorldPosition().x - key->GetWorldPosition().x), 2.0f);
			distance.z = std::powf((player_->GetWorldPosition().z - key->GetWorldPosition().z), 2.0f);

			float colissionDistance = sqrtf(distance.x + distance.y + distance.z);
			if (colissionDistance > player_->GetRadius() + key->GetRadius()) {
				return;
			}

			if (colissionDistance <= player_->GetRadius() + key->GetRadius()) {

#ifdef _DEBUG
				ImGui::Begin("KeyCollision");
				ImGui::End();
#endif // _DEBUG

				if (Input::GetInstance()->IsPushKey(DIK_SPACE) == true) {
					player_->AddHaveKeyQuantity();
				}


			}
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
	firstPersonRotate_ = theta;
	//+が左回り
	const float ROTATE_OFFSET = 0.025f;
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		theta += ROTATE_OFFSET;
		
	}
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		theta -= ROTATE_OFFSET;
		
	}

	lightDirection_.x = std::cosf(theta);
	lightDirection_.z = std::sinf(theta);
	const float LIGHT_HEIGHT = 0.5f;
	lightPosition = Add(player_->GetWorldPosition(), {0.0f, LIGHT_HEIGHT,0.0f});

	spotLight_.position_ = lightPosition;
	spotLight_.intensity_ = intencity_;
	spotLight_.direction_ = lightDirection_;
	spotLight_.distance_ = distance_;
	spotLight_.decay_ = decay_;
	spotLight_.cosFallowoffStart_ = fallOff_;
	spotLight_.cosAngle_ = cosAngle_;




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

		camera_.rotate_.x = 0.0f;
		camera_.rotate_.z = 0.0f;

		//回り方が少し違うので注意
		camera_.rotate_.y = -(firstPersonRotate_)+std::numbers::pi_v<float>/2.0f;
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


	//鍵
	for (Key* key : keyes_) {
		key->Update();
	}


	//プレイヤーの更新
	player_->Update();

	KeyCollision();
	//
	keyes_.remove_if([](Key* key) {
		if (key->GetIsPickUp() == true) {
			delete key;
			return true;
		}
		return false;
	});
	//地面
	groundWorldTransform_.Update();


	material_.Update();
	spotLight_.Update();
	directionalLight_.Update();
	//ライト
	lightCollision_->Update(player_->GetWorldPosition());
	

	//当たり判定
	collisionManager_->CheckAllCollision();



	//敵がが生存していなかったら消す
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsAlive() == false) {
			delete enemy;
			return true;
		}
		return false;
	});


#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::InputFloat3("Rotate", &camera_.rotate_.x);
	ImGui::InputFloat("Theta", &theta);
	ImGui::InputFloat("FirstRotate", &firstPersonRotate_);
	
	ImGui::End();


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

void SampleScene::DrawSpriteBack(){

	//sprite_->Draw();
}

void SampleScene::PreDrawPostEffectFirst(){
	
	
	back_->PreDraw();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::DrawObject3D() {
	
	//プレイヤー
	//1人称だったらモデルは表示させない
	//自分の目から自分の全身が見えるのはおかしいからね
	if (viewOfPoint_ != FirstPerson) {
		player_->Draw(camera_, material_, spotLight_);

	}
	
	//地面
	ground_->Draw(groundWorldTransform_, camera_,material_, spotLight_);
	//敵
	for (Enemy* enemy : enemys_) {
		enemy->Draw(camera_,spotLight_);
	}

	//鍵
	for (Key* key : keyes_) {
		key->Draw(camera_, spotLight_);
	}

	lightCollision_->Draw(camera_, material_, spotLight_);

	
}



void SampleScene::DrawPostEffect(){
	
	
	back_->Draw();
}

void SampleScene::DrawSprite(){
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	//敵
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}

	for (Key* key : keyes_) {
		delete  key;
	}


	delete lightCollision_;
}