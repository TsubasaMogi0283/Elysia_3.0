#include "SampleScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "SampleScene2/SampleScene2.h"

#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>
#include <TextureManager.h>
#include <SingleCalculation.h>


void SampleScene::Initialize() {

#pragma region フェード
	uint32_t fadeTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Back/White.png");
	fadeSprite_.reset(Sprite::Create(fadeTextureHandle, { .x = 0.0f,.y = 0.0f }));
	fadeTransparency_ = 1.0f;

	isFadeIn = true;
	isFadeOut_ = false;

#ifdef _DEBUG
	isFadeIn = false;
	isFadeOut_ = false;

#endif // _DEBUG

#pragma endregion


	
#pragma region オブジェクト
	objectManager_ = new ObjectManager();
	objectManager_->Initialize();

	#pragma region 地面
	uint32_t groundModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Ground", "Ground.obj");
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModelHandle);
	StageRect stageRect = ground_->GetStageRect();
	#pragma endregion

	
	#pragma region ゲート
	uint32_t gateModelhandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Gate","Gate.obj");
	gate_ = std::make_unique<Gate>();
	gate_->Initialize(gateModelhandle);




	

	uint32_t escapeTexturehandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Escape/EscapeText.png");
	escapeText_.reset(Sprite::Create(escapeTexturehandle, { .x = 0.0f,.y = 0.0f }));
	//最初は非表示にする
	escapeText_->SetInvisible(true);
	#pragma endregion

#pragma endregion

#pragma region プレイヤー
	player_ = new Player();
	//ステージ
	player_->SetStageRect(stageRect);
	player_->SetObjectManager(objectManager_);
	player_->Initialize();
	player_->SetIsAbleToControll(false);

	playerDirection_ = { 0.0f,0.0f,0.0f };
	isPlayerMoveKey_ = false;
	bTriggerTime_ = 0;
	isBTrigger_ = false;

	//初期は1人称視点
	viewOfPoint_ = FirstPerson;

#pragma endregion


#pragma region 鍵
	uint32_t keyModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","Cube.obj");
	
	keyManager_ = std::make_unique<KeyManager>();
	keyManager_->Initialize(keyModelHandle);
	
#pragma endregion
	
	uint32_t skydomeModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Game/Skydome","Skydome.obj");
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModelHandle);



	#pragma region 敵
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "Ghost.gltf");
#ifdef _DEBUG
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD2_Enemy", "TD2_Enemy.obj");

#endif // _DEBUG

	
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_);
	enemyManager_->SetStageRectangle(stageRect);
	enemyManager_->Initialize(enemyModelHandle_);
	#pragma endregion



	#pragma region ライト確認用のタワー
	uint32_t debugTowerModelhandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Tower", "Tower.obj");
	debugTower_.reset(Model::Create(debugTowerModelhandle));
	debugTowerWorldTransform_.Initialize();
	debugTowerWorldTransform_.translate_ = { .x = 1.0f,.y = 0.0f,.z = 2.0f };
	#pragma endregion
	
	
	
	
	#pragma region カメラ
	//カメラ
	camera_.Initialize();
	camera_.translate_.y = 1.0f;
	camera_.translate_.z = -15.0f;
	camera_.rotate_.y = std::numbers::pi_v<float> / 2.0f;
	cameraPosition_ = camera_.translate_;
	
	CAMERA_POSITION_OFFSET = { 0.0f,2.0f,0.0f };
	
	thirdPersonViewOfPointRotate_ = { 0.6f,0.0f,0.0f };
	cameraThirdPersonViewOfPointPosition_ = { 0.0f,25.0f,-35.0f };
	
	
	#pragma endregion
	
	
	//プレイヤーのライト
	uint32_t weaponLightModel = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	lightCollision_ = new LightWeapon();
	lightCollision_->Initialize(weaponLightModel);
	
	
	
	#pragma region 扇の当たり判定用の球
	debugFanCollisionSphereModel_.reset(Model::Create(weaponLightModel));
	debugFanCollisionSphereWorldTransform_.Initialize();
	debugFanCollisionSphereWorldTransform_.translate_ = { .x = 0.0f,.y = 0.0f,.z = 7.0f };
	debugFanCollisionSphereMaterial_.Initialize();
	debugFanCollisionSphereMaterial_.lightingKinds_ = Spot;
	debugFanCollisionSphereMaterial_.color_ = { .x = 0.0f,.y = 1.0f,.z = 0.0f,.w = 1.0f };
	
	
	
	#pragma endregion
	


#pragma region 説明
	uint32_t explanationTextureHandle[EXPLANATION_QUANTITY_] = {};
	explanationTextureHandle[0] = TextureManager::GetInstance()->LoadTexture("Resources/Game/Explanation/Explanation1.png");
	explanationTextureHandle[1] = TextureManager::GetInstance()->LoadTexture("Resources/Game/Explanation/Explanation2.png");

	for (uint32_t i = 0; i < EXPLANATION_QUANTITY_; ++i) {
		explanation_[i].reset(Sprite::Create(explanationTextureHandle[i], {.x=0.0f,.y=0.0f}));
	}
	

	uint32_t spaceToNextTextureHandle[SPACE_TO_NEXT_QUANTITY_] = {};
	spaceToNextTextureHandle[0] = TextureManager::GetInstance()->LoadTexture("Resources/Game/Explanation/ExplanationNext1.png");
	spaceToNextTextureHandle[1] = TextureManager::GetInstance()->LoadTexture("Resources/Game/Explanation/ExplanationNext2.png");

	for (uint32_t i = 0; i < SPACE_TO_NEXT_QUANTITY_; ++i) {
		spaceToNext_[i].reset(Sprite::Create(spaceToNextTextureHandle[i], { .x=0.0f,.y = 0.0f }));
	}

	
	howToPlayTextureNumber_ = 0;

	//常時表示
	//操作
	uint32_t operationTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Operation/Operation.png");
	operation_.reset(Sprite::Create(operationTextureHandle, {.x=0.0f,.y=0.0f}));
	isGamePlay_ = false;

	uint32_t pickUpTextureManager = TextureManager::GetInstance()->LoadTexture("Resources/Game/Key/PickUpKey.png");
	pickUpKey_.reset(Sprite::Create(pickUpTextureManager, { .x = 0.0f,.y = 0.0f }));
	
#pragma endregion

	isGamePlay_ = false;
	isExplain_ = false;
	

	collisionManager_ = std::make_unique<CollisionManager>();
	
	theta_ = std::numbers::pi_v<float> / 2.0f;
	
	back_ = std::make_unique< BackText>();
	back_->Initialize();
	
	material_.Initialize();
	material_.lightingKinds_ = Spot;
	
	//懐中電灯
	flashLight_ = std::make_unique<FlashLight>();
	flashLight_->Initialize();
	


#ifdef _DEBUG
	isFadeIn = false;
	isGamePlay_ = true;
#endif // _DEBUG


}



void SampleScene::KeyCollision(){

	//鍵
	std::list<Key*> keyes = keyManager_->GetKeyes();
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

				key->SetIsPrePickUp(true);

				//
				if (Input::GetInstance()->IsPushKey(DIK_SPACE) == true) {
					//プレイヤーの持っているか鍵の数が増える
					player_->AddHaveKeyQuantity();
					//鍵が取得される
					key->PickedUp();
				}

				//Bボタンを押したとき
				if (Input::GetInstance()->GetJoystickState(joyState) == true){
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
						bTriggerTime_ += 1;

					}
					if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
						bTriggerTime_ = 0;
					}

					if (bTriggerTime_ == 1) {
						//プレイヤーの持っているか鍵の数が増える
						player_->AddHaveKeyQuantity();
						//鍵が取得される
						key->PickedUp();
					}
				}

			}
			else {
				key->SetIsPrePickUp(false);
			}


		}
	}

	//listにあるKeyの中にある「isPrePickUp_」のどれか一つでもtrueになっていたらtrueを返す
	isAbleToPickUpKey_ = std::any_of(keyes.begin(), keyes.end(), [](Key* key) {
		return key->GetIsPrePickUp() == true;
	});


#ifdef _DEBUG
	ImGui::Begin("KeyPickUp"); 
	ImGui::Checkbox("IsPickUp", &isAbleToPickUpKey_);
	ImGui::End();
#endif // _DEBUG


}

void SampleScene::ObjectCollision(){
	Vector3 playerPosition = player_->GetWorldPosition();
	
	//std::list <DemoObject*> demoObjects = objectManager_->GetDemoObjets();
	//for (DemoObject* demoObject : demoObjects) {
	//	//ワールド座標
	//	Vector3 objectPosition = demoObject->GetWorldPosition();

	//	//オブジェクトは動く必要がないのでプレイヤーだけで大丈夫
	//	
	//	//オブジェクトとプレイヤーとの差分ベクトル
	//	Vector3 objectPlayerDifference = VectorCalculation::Subtract(objectPosition, playerPosition);
	//	float enemyAndEnemyDistance = sqrtf(std::powf(objectPlayerDifference.x, 2.0f) + std::powf(objectPlayerDifference.y, 2.0f) + std::powf(objectPlayerDifference.z, 2.0f));

	//	// 正射影ベクトルを求める
	//	// 上のベクトルを射影する
	//	Vector3 projectObjectPlayerDifference = VectorCalculation::Project(objectPlayerDifference, playerDirection_);
	//	Vector3 differenceEnemyAndProject = VectorCalculation::Subtract(objectPlayerDifference, projectObjectPlayerDifference);
	//	float projectDistance = sqrtf(std::powf(differenceEnemyAndProject.x, 2.0f) + std::powf(differenceEnemyAndProject.y, 2.0f) + std::powf(differenceEnemyAndProject.z, 2.0f));



	//	//射影ベクトルがそれぞれの半径より短くなった場合
	//	if (projectDistance < player_->GetRadius() + demoObject->GetRadius()) {

	//		//内積
	//		//進行方向の前にいると+
	//		Vector3 normalizedObjectAndPlayer = VectorCalculation::Normalize(objectPlayerDifference);
	//		float dot = SingleCalculation::Dot(playerDirection_, normalizedObjectAndPlayer);

	//		//進行方向にいた場合
	//		if ((enemyAndEnemyDistance < player_->GetRadius() + demoObject->GetRadius()) && dot > 0.0f) {

	//		}

	//	}


	//	
	//}

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	//フレーム初めに
	//コリジョンリストのクリア
	collisionManager_->ClearList();

	fadeSprite_->SetTransparency(fadeTransparency_);
	//フェードイン
	if (isFadeIn==true) {
		const float FADE_IN_INTERVAL = 0.01f;
		fadeTransparency_ -= FADE_IN_INTERVAL;
		
		//完全に透明になったらゲームが始まる
		if (fadeTransparency_ < 0.0f) {
			fadeTransparency_ = 0.0f;
			isFadeIn = false;
			isExplain_ = true;
			howToPlayTextureNumber_ = 1;
		}
	}


	//ゲーム
	if (isFadeIn == false && isFadeOut_ == false) {
		fadeTransparency_ = 0.0f;
		
		if (isExplain_ == true) {
			if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
				++howToPlayTextureNumber_;
			}
			//Bボタンを押したとき
			if (Input::GetInstance()->GetJoystickState(joyState) == true) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += 1;

				}
				if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
					bTriggerTime_ = 0;
				}

				if (bTriggerTime_ == 1) {
					++howToPlayTextureNumber_;
				}
			}

			if (howToPlayTextureNumber_ > 2) {
				isExplain_ = false;
				isGamePlay_ = true;
			}
		}
		if (isGamePlay_ == true) {
			player_->SetIsAbleToControll(true);

			//操作説明を追加
			isDisplayUI_ = true;

			//敵
			enemyManager_->Update();

			//敵を消す
			enemyManager_->DeleteEnemy();
		}
		
		

#pragma region 回転

#pragma region Y軸に旋回

		//+が左回り
		const float ROTATE_OFFSET = 0.025f;
		//左を向く
		if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
			theta_ += ROTATE_OFFSET;
			isRotateYKey_ = true;
		}
		//右を向く
		if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
			theta_ -= ROTATE_OFFSET;
			isRotateYKey_ = true;
		}

		

		//2πより大きくなったら0にまた戻す
		if (theta_ > 2.0f * std::numbers::pi_v<float>) {
			theta_ = 0.0f;
		}
		//-2πより大きくなったら0にまた戻す
		if (theta_ < -2.0f * std::numbers::pi_v<float>) {
			theta_ = 0.0f;
		}

#pragma endregion

#pragma region X軸に旋回

		
		//上を向く
		if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
			originPhi_ -= ROTATE_OFFSET;
			isRotateXKey_ = true;
		}
		//下を向く
		if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
			originPhi_ += ROTATE_OFFSET;
			isRotateXKey_ = true;
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

#pragma region コントローラーの回転

		isRotateXKey_ = false;
		isRotateYKey_ = false;

		//コントローラーがある場合
		if (Input::GetInstance()->GetJoystickState(joyState) == true) {
			const float MOVE_LIMITATION = 0.02f;

			//キーボード入力していない時
			if (isRotateYKey_ == false && isRotateXKey_ == false) {

				//入力
				float rotateMoveX = (float)joyState.Gamepad.sThumbRY / SHRT_MAX * ROTATE_OFFSET;
				float rotateMoveY = (float)joyState.Gamepad.sThumbRX / SHRT_MAX * ROTATE_OFFSET;
				
				//勝手に動くので制限を掛ける
				if (rotateMoveY < MOVE_LIMITATION && rotateMoveY > -MOVE_LIMITATION) {
					rotateMoveY = 0.0f;
				}
				if (rotateMoveX < MOVE_LIMITATION && rotateMoveX > -MOVE_LIMITATION) {
					rotateMoveX = 0.0f;
				}

				//補正後の値を代入する
				theta_ -= rotateMoveY;
				originPhi_ -= rotateMoveX;
			}



		}


#pragma endregion

#pragma region プレイヤーの移動

		playerDirection_ = { 0.0f,0.0f,0.0f };
		isPlayerMoveKey_ = false;
		//移動
		if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
			playerDirection_.x = std::cosf(theta_ - std::numbers::pi_v<float> / 2.0f);
			playerDirection_.z = std::sinf(theta_ - std::numbers::pi_v<float> / 2.0f);
			isPlayerMoveKey_ = true;
		}
		if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
			playerDirection_.x = std::cosf(theta_ + std::numbers::pi_v<float> / 2.0f);
			playerDirection_.z = std::sinf(theta_ + std::numbers::pi_v<float> / 2.0f);
			isPlayerMoveKey_ = true;
		}
		if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
			playerDirection_.x = std::cosf(theta_);
			playerDirection_.z = std::sinf(theta_);
			isPlayerMoveKey_ = true;
		}
		if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
			playerDirection_.x = std::cosf(theta_ + std::numbers::pi_v<float>);
			playerDirection_.z = std::sinf(theta_ + std::numbers::pi_v<float>);
			isPlayerMoveKey_ = true;
		}
#ifdef _DEBUG
		ImGui::Begin("Player");
		ImGui::InputFloat3("Direction", &playerDirection_.x);
		ImGui::End();

#endif // _DEBUG


		//コントローラーがある場合
		if (Input::GetInstance()->GetJoystickState(joyState) == true) {
			//キーボード入力していない時に受け付ける
			if (isPlayerMoveKey_ == false) {


				//コントローラーの入力
				bool isInput = false; 
				Vector3 leftStickInput = {
					.x = (static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * 1.0f),
					.z = (static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * 1.0f),
				};
				


				//デッドゾーン
				const float DEAD_ZONE = 0.1f;
				if (leftStickInput.x < DEAD_ZONE && leftStickInput.x > -DEAD_ZONE) {
					leftStickInput.x = 0.0f;
				}
				else {
					isInput = true;
				}
				if (leftStickInput.z < DEAD_ZONE && leftStickInput.z > -DEAD_ZONE) {
					leftStickInput.z = 0.0f;
				}
				else {
					isInput = true;
				}

				//入力されていたら計算
				if (isInput == true) {
					//角度を求める
					float radian = std::atan2f(leftStickInput.z, leftStickInput.x);
					//値を0～2πに直してtheta_に揃える
					if (radian < 0.0f) {
						radian += 2.0f * std::numbers::pi_v<float>;
					}
					const float OFFSET = std::numbers::pi_v<float> / 2.0f;
					float resultTheta = theta_ + radian-OFFSET;



					//向きを代入
					playerDirection_.x = std::cosf(resultTheta);
					playerDirection_.z = std::sinf(resultTheta);

				}
				

			}
		}


		//プレイヤーの動く方向を入れる
		player_->SetMoveDirection(playerDirection_);

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


		


		Fan3D fan = flashLight_->GetFan3D();
		//エネミーをコリジョンマネージャーに追加
		std::list<Enemy*> enemyes = enemyManager_->GetEnemyes();
		for (std::list<Enemy*>::iterator it = enemyes.begin(); it != enemyes.end();) {
			Enemy* enemy = *it;
			if (enemy != nullptr) {
				collisionManager_->RegisterList(enemy);


				if (IsFanCollision(fan, enemy->GetWorldPosition())) {
	
					enemy->OnCollision();


					#ifdef _DEBUG
					ImGui::Begin("FanCollsion");
					ImGui::End();
					#endif // _DEBUG
	
					
				}
			}
			it++;
		}



		#pragma region 視点

		//1人称視点へ変更
		if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
			viewOfPoint_ = FirstPerson;
		}
		

		//3人称視点へ変更
		else if (Input::GetInstance()->IsTriggerKey(DIK_3) == true) {
			viewOfPoint_ = ThirdPersonBack;
		}

		//コントローラーだと
		//十字ボタンで切り替えのが良いかも
		//マイクラはそれだったから

		//1人称
		if (viewOfPoint_ == FirstPerson) {

			//もとに戻す
			camera_.rotate_.x = -phi;
			camera_.rotate_.y = -(theta_)+std::numbers::pi_v<float> / 2.0f;
			camera_.rotate_.z = 0.0f;

			camera_.translate_ = VectorCalculation::Add(playerPosition, CAMERA_POSITION_OFFSET);

		}
		else if (viewOfPoint_ == ThirdPersonBack) {


			camera_.rotate_ = thirdPersonViewOfPointRotate_;
			//camera_.rotate_ = {0.2f,0.0f,0.0f};

			camera_.translate_ = VectorCalculation::Add(playerPosition, VectorCalculation::Add(cameraThirdPersonViewOfPointPosition_, cameraTranslate));
			//camera_.translate_ = {0.0f,5.0f,-15.0f};

		}


#pragma endregion


		#pragma region 鍵の取得処理
		uint32_t keyQuantity = keyManager_->GetKeyQuantity();
		//鍵が0より多ければ通る
		if (keyQuantity > 0) {
			KeyCollision();
		}
		else {
			isAbleToPickUpKey_ = false;
		}

		#pragma endregion


		//ゲート
		if (gate_->isCollision(playerPosition)) {
#ifdef _DEBUG
			ImGui::Begin("InSpaceGate");
			ImGui::End();

#endif // _DEBUG

			//3個取得したら脱出できる
			uint32_t playerKeyQuantity = player_->GetHavingKey();
			if (playerKeyQuantity >= keyManager_->GetMaxKeyQuantity()) {
				escapeText_->SetInvisible(false);



				//コントローラーのBボタンを押したら脱出のフラグがたつ
				if (Input::GetInstance()->GetJoystickState(joyState) == true) {

					//Bボタンを押したとき
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
						bTriggerTime_ += 1;

					}
					if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
						bTriggerTime_ = 0;
					}

					if (bTriggerTime_ == 1) {
						//脱出
						isEscape_ = true;
					}

				}
				//SPACEキーを押したら脱出のフラグがたつ
				if (Input::GetInstance()->IsPushKey(DIK_SPACE) == true) {
					//脱出
					isEscape_ = true;
				}
			}
		}
		else {
			escapeText_->SetInvisible(true);
		}

		//脱出
		if (isEscape_ == true) {
			isFadeOut_ = true;
			
		}
		#pragma endregion

		
		//鍵
		keyManager_->Update();

		//オブジェクトの当たり判定
		ObjectCollision();


		//ライト
		Vector3 lightDirection = flashLight_->GetDirection();
		lightCollision_->Update(player_->GetWorldPosition(), lightDirection);

		//更新
		material_.Update();

		//当たり判定
		collisionManager_->CheckAllCollision();

#ifdef _DEBUG
		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
		ImGui::SliderFloat3("Traslate", &cameraTranslate.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("PosOffset", &CAMERA_POSITION_OFFSET.x, -30.0f, 30.0f);
		ImGui::InputFloat("Theta", &theta_);
		ImGui::InputFloat("Phi", &phi);
		ImGui::End();
#endif

	}
	
	//ホワイトアウト
	if (isFadeOut_ == true) {
		escapeText_->SetInvisible(true);

		player_->SetIsAbleToControll(false);
		const float FADE_OUT_INTERVAL = 0.01f;
		fadeTransparency_ += FADE_OUT_INTERVAL;
		fadeSprite_->SetTransparency(fadeTransparency_);

		if (fadeTransparency_ > 1.0f) {
			gameManager->ChangeScene(new SampleScene2());
			return;
		}
		
	}

	
	//カメラの更新
	camera_.Update();

	//オブジェクトマネージャーの更新
	objectManager_->Update();

	
	//地面
	ground_->Update();

	//門
	gate_->Update();

	//天球
	skydome_->Update();

	//プレイヤーの更新
	player_->Update();

	//ライト確認用のタワー
	debugTowerWorldTransform_.Update();

	debugFanCollisionSphereWorldTransform_.Update();
	debugFanCollisionSphereMaterial_.Update();

}

void SampleScene::DrawSpriteBack(){

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
	//ゲート
	gate_->Draw(camera_, spotLight);
	//敵
	enemyManager_->Draw(camera_, spotLight);
	//天球
	skydome_->Draw(camera_);

	//懐中電灯
	flashLight_->Draw(camera_);

	//ステージオブジェクト
	objectManager_->Draw(camera_, spotLight);

	//タワー
	//debugTower_->Draw(debugTowerWorldTransform_, camera_, material_, spotLight);
	
	//鍵
	keyManager_->DrawObject3D(camera_, spotLight);

#ifdef _DEBUG
	lightCollision_->Draw(camera_, spotLight);

	//debugFanCollisionSphereModel_->Draw(debugFanCollisionSphereWorldTransform_,camera_, debugFanCollisionSphereMaterial_,spotLight);

#endif // _DEBUG

	

}



void SampleScene::DrawPostEffect(){
	
	back_->Draw();
}

void SampleScene::DrawSprite(){
	
	//説明
	if (howToPlayTextureNumber_ == 1) {
		explanation_[0]->Draw();
		spaceToNext_[0]->Draw();
	}
	if (howToPlayTextureNumber_ == 2) {
		explanation_[1]->Draw();
		spaceToNext_[1]->Draw();
	}
	
	//UIを表示するかどうか
	if (isDisplayUI_ == true) {
		//操作説明
		operation_->Draw();

		//鍵の取得
		if (isAbleToPickUpKey_ == true) {
			pickUpKey_->Draw();
		}
		
		//鍵
		uint32_t keyQuantity = player_->GetHavingKey();
		keyManager_->DrawSprite(keyQuantity);

		//脱出
		escapeText_->Draw();
	}

	//フェード
	if (isFadeIn == true || isFadeOut_ == true) {
		fadeSprite_->Draw();
	}


}



SampleScene::~SampleScene() {
	delete player_;
	delete lightCollision_;
	delete objectManager_;
}