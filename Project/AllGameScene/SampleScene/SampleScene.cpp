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
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModelHandle);

#pragma region 鍵
	uint32_t keyModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","Cube.obj");

	keyManager_ = std::make_unique<KeyManager>();
	keyManager_->Initialize(keyModelHandle);

#pragma endregion


#pragma region 敵
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD2_Enemy", "TD2_Enemy.obj");
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize(enemyModelHandle_);


#pragma endregion



	uint32_t debugTowerModelhandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Tower", "Tower.obj");
	debugTower_.reset(Model::Create(debugTowerModelhandle));
	debugTowerWorldTransform_.Initialize();




	

	//カメラ
	camera_.Initialize();
	camera_.translate_.y = 1.0f;
	camera_.translate_.z = -15.0f;
	camera_.rotate_.y = std::numbers::pi_v<float> / 2.0f;
	cameraPosition_ = camera_.translate_;

	CAMERA_POSITION_OFFSET = { 0.0f,3.0f,0.0f };

	thirdPersonViewOfPointRotate_ = { 0.6f,0.0f,0.0f };
	cameraThirdPersonViewOfPointPosition_ = { 0.0f,25.0f,-35.0f };





	//プレイヤーのライト
	uint32_t weaponLightModel = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	lightCollision_ = new LightWeapon();
	lightCollision_->Initialize(weaponLightModel);


	collisionManager_ = std::make_unique<CollisionManager>();


	theta_ = std::numbers::pi_v<float>/2.0f;

	back_ = std::make_unique< BackText>();
	back_->Initialize();

	material_.Initialize();
	material_.lightingKinds_ = Spot;
	//material_.lightingKinds_ = Directional;

	//懐中電灯
	flashLight_ = std::make_unique<FlashLight>();
	flashLight_->Initialize();

	directionalLight_.Initialize();
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

	//鍵
	auto keyes = keyManager_->GetKeyes();
	for (Key* key : keyes) {

		//勿論取得されていない時だけ受け付ける
		if (key->GetIsPickUp() == false) {
			//判定は円で良いかも
			Vector3 distance = {};
			distance.x = std::powf((player_->GetWorldPosition().x - key->GetWorldPosition().x), 2.0f);
			distance.z = std::powf((player_->GetWorldPosition().z - key->GetWorldPosition().z), 2.0f);

			float colissionDistance = sqrtf(distance.x + distance.y + distance.z);
			

			//範囲内にいれば入力を受け付ける
			if (colissionDistance <= player_->GetRadius() + key->GetRadius()) {

#ifdef _DEBUG
				ImGui::Begin("KeyCollision");
				ImGui::End();
#endif 

				//
				if (Input::GetInstance()->IsPushKey(DIK_SPACE) == true) {
					//プレイヤーの持っているか鍵の数が増える
					player_->AddHaveKeyQuantity();
					//鍵が取得される
					key->PickedUp();
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




#pragma region 回転
	//+が左回り
	const float ROTATE_OFFSET = 0.025f;
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		theta_ += ROTATE_OFFSET;
		isRotateKey_ = true;
		
	}
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		theta_ -= ROTATE_OFFSET;
		isRotateKey_ = true;
		
	}

	//コントローラーがある場合
	XINPUT_STATE joyState{};
	isRotateKey_ = false;
	if (Input::GetInstance()->GetJoystickState(joyState) == true) {
		if (isRotateKey_ == false) {
			//やっぱりこっちも逆だね☆
			float rotateMove = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * ROTATE_OFFSET;
			
			
			//何か勝手に動いちゃうので制限を掛ける
			const float MOVE_LIMITATION = 0.01f;
			if (rotateMove < MOVE_LIMITATION && rotateMove > -MOVE_LIMITATION) {
				rotateMove = 0.0f;
			}
			theta_ -= rotateMove;
			

			

		}


	}




	const float CAMERA_ROTATE_AMOUNT = ROTATE_OFFSET;
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		originPhi_ -= CAMERA_ROTATE_AMOUNT;
	}
	//下を向く
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		originPhi_ += CAMERA_ROTATE_AMOUNT;
	}

	if (Input::GetInstance()->GetJoystickState(joyState) == true) {
		if (isRotateXKey_ == false) {
			float rotateMove = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * ROTATE_OFFSET;


			//勝手に動いちゃうので制限を掛ける
			const float MOVE_LIMITATION = 0.01f;
			if (rotateMove < MOVE_LIMITATION && rotateMove > -MOVE_LIMITATION) {
				rotateMove = 0.0f;
			}
			originPhi_ -= rotateMove;




		}


	}



	//±π/6くらいに制限を掛けておきたい
	//それ以下以上だと首が大変なことになっているように見えるからね
	if (originPhi_ > std::numbers::pi_v<float> / 6.0f) {
		originPhi_ = std::numbers::pi_v<float> / 6.0f;
	}
	if (originPhi_ < -std::numbers::pi_v<float> / 6.0f) {
		originPhi_ = -std::numbers::pi_v<float> / 6.0f;
	}

#pragma endregion

	
	//数学とプログラムで回る向きが違うことに煩わしさを感じます・・
	//無理矢理直して楽になろう！！
	float phi = -originPhi_;

	//懐中電灯
	Vector3 playerPosition = player_->GetWorldPosition();
	flashLight_->SetPlayerPosition(playerPosition);
	flashLight_->SetTheta(theta_);
	flashLight_->SetPhi(phi);
	flashLight_->Update();



	collisionManager_->RegisterList(lightCollision_);

	auto enemyes = enemyManager_->GetEnemyes();

	for (auto it = enemyes.begin(); it != enemyes.end();) {
		Enemy* enemy = *it;
		if (enemy != nullptr) {
			collisionManager_->RegisterList(enemy);
		}
		it++;
	}


	//1人称視点へ変更
	if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
		viewOfPoint_ = FirstPerson;
	}
	//3人称視点へ変更
	else if (Input::GetInstance()->IsTriggerKey(DIK_3) == true) {
		viewOfPoint_ = ThirdPersonBack;
	}

	//コントローラーだと
	//十字ボタンが良いかも
	//マイクラはそれだったから

	//1人称
	if (viewOfPoint_ == FirstPerson) {

		
		
		//回り方が少し違うので注意
		//何か嫌だね
		camera_.rotate_.x = -phi;
		camera_.rotate_.y = -(theta_)+std::numbers::pi_v<float>/2.0f;
		camera_.rotate_.z = 0.0f;
		camera_.translate_ = Add(player_->GetWorldPosition(), CAMERA_POSITION_OFFSET);

	}
	else if (viewOfPoint_ == ThirdPersonBack) {

#ifdef _DEBUG
		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Pos", &cameraThirdPersonViewOfPointPosition_.x, -70.0f, 40.0f);
		ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);

		ImGui::End();
#endif 

		camera_.rotate_ = thirdPersonViewOfPointRotate_;
		camera_.translate_ = Add(player_->GetWorldPosition(), cameraThirdPersonViewOfPointPosition_);
	}


	//カメラの更新
	camera_.Update();



	//敵
	enemyManager_->Update();

	//敵同士
	CheckCollision(enemyes);



	debugTowerWorldTransform_.Update();
	
	

	//プレイヤーの更新
	player_->Update();

	//鍵
	keyManager_->Update();

	//鍵の取得処理
	uint32_t keyQuantity = keyManager_->GetKeyQuantity();
	//鍵が0より多ければ通る
	if (keyQuantity > 0) {
		KeyCollision();
	}
	


	//地面
	ground_->Update();
	
	//ライト
	Vector3 lightDirection = flashLight_->GetDirection();
	lightCollision_->Update(player_->GetWorldPosition(), lightDirection);


	//更新
	material_.Update();
	directionalLight_.Update();
	
	//当たり判定
	collisionManager_->CheckAllCollision();



	enemyManager_->DeleteEnemy();


#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::InputFloat3("Rotate", &camera_.rotate_.x);
	ImGui::SliderFloat3("PosOffset", &CAMERA_POSITION_OFFSET.x, -30.0f, 30.0f);
	ImGui::InputFloat("Theta", &theta_);
	ImGui::InputFloat("Phi", &phi);
	
	ImGui::End();



#endif

	
}

void SampleScene::DrawSpriteBack(){

	//sprite_->Draw();
}

void SampleScene::PreDrawPostEffectFirst(){
	
	
	back_->PreDraw();
}


void SampleScene::DrawObject3D() {
	
	//懐中電灯を取得
	SpotLight spotLight = flashLight_->GetSpotLight();

	//プレイヤー
	//1人称だったらモデルは表示させない
	//自分の目から自分の全身が見えるのはおかしいからね
	if (viewOfPoint_ != FirstPerson) {
		player_->Draw(camera_, material_, spotLight);

	}
	
	//地面
	ground_->Draw(camera_, spotLight);
	//敵
	enemyManager_->Draw(camera_, spotLight);

	

	lightCollision_->Draw(camera_, spotLight);

	debugTower_->Draw(debugTowerWorldTransform_, camera_, material_, spotLight);
	

	//鍵
	keyManager_->DrawObject3D(camera_, spotLight);

}



void SampleScene::DrawPostEffect(){
	
	back_->Draw();
}

void SampleScene::DrawSprite(){
	//鍵
	uint32_t keyQuantity = player_->GetHavingKey();
	keyManager_->DrawSprite(keyQuantity);


}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
	delete lightCollision_;
}