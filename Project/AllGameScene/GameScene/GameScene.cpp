#include "GameScene.h"
#include <imgui.h>
#include <Input.h>
#include <AdjustmentItems.h>
#include "SampleScene2/SampleScene2.h"
#include "LoseScene/LoseScene.h"

#include "ModelManager.h"
#include "AnimationManager.h"
#include <numbers>
#include <TextureManager.h>
#include <SingleCalculation.h>



GameScene::GameScene(){
	input_ = Input::GetInstance();
}

void GameScene::Initialize() {

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



	uint32_t escapeTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Escape/EscapeText.png");
	escapeText_.reset(Sprite::Create(escapeTextureHandle, { .x = 0.0f,.y = 0.0f }));
	//最初は非表示にする
	escapeText_->SetInvisible(true);
	#pragma endregion

#pragma endregion

#pragma region プレイヤー
	player_ = std::make_unique<Player>();
	//ステージ
	player_->SetStageRect(stageRect);
	player_->Initialize();
	player_->SetIsAbleToControll(false);

	playerPosition_ = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	playerMoveDirection_ = { 0.0f,0.0f,0.0f };
	isPlayerMoveKey_ = false;
	bTriggerTime_ = 0;
	isBTrigger_ = false;

	



#pragma endregion


#pragma region 鍵
	uint32_t keyModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/key","Key.obj");
	
	keyManager_ = std::make_unique<KeyManager>();
	keyManager_->Initialize(keyModelHandle);
	
#pragma endregion
	
	uint32_t skydomeModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Game/Skydome","Skydome.obj");
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModelHandle);



	#pragma region 敵
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "Ghost.gltf");
	uint32_t strongEnemyModelHandle= ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "StrongGhost.gltf");

#ifdef _DEBUG
	enemyModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube", "Cube.obj");
#endif // _DEBUG

	
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->SetObjectManager(objectManager_);
	enemyManager_->SetStageRectangle(stageRect);
	enemyManager_->Initialize(enemyModelHandle_, strongEnemyModelHandle);
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

	for (uint32_t i = 0u; i < EXPLANATION_QUANTITY_; ++i) {
		explanation_[i].reset(Sprite::Create(explanationTextureHandle[i], {.x=0.0f,.y=0.0f}));
	}
	

	uint32_t spaceToNextTextureHandle[SPACE_TO_NEXT_QUANTITY_] = {};
	spaceToNextTextureHandle[0] = TextureManager::GetInstance()->LoadTexture("Resources/Game/Explanation/ExplanationNext1.png");
	spaceToNextTextureHandle[1] = TextureManager::GetInstance()->LoadTexture("Resources/Game/Explanation/ExplanationNext2.png");

	for (uint32_t i = 0; i < SPACE_TO_NEXT_QUANTITY_; ++i) {
		spaceToNext_[i].reset(Sprite::Create(spaceToNextTextureHandle[i], { .x=0.0f,.y = 0.0f }));
	}

	
	howToPlayTextureNumber_ = 0u;

	//常時表示
	//操作
	uint32_t operationTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Operation/Operation.png");
	operation_.reset(Sprite::Create(operationTextureHandle, {.x=20.0f,.y=0.0f}));
	isGamePlay_ = false;

	uint32_t pickUpTextureManager = TextureManager::GetInstance()->LoadTexture("Resources/Game/Key/PickUpKey.png");
	pickUpKey_.reset(Sprite::Create(pickUpTextureManager, { .x = 0.0f,.y = 0.0f }));
	
#pragma endregion

#pragma region UI
	uint32_t playerHPTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Player/PlayerHP.png");
	uint32_t playerHPBackFrameTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Player/PlayerHPBack.png");
	const Vector2 INITIAL_POSITION = { .x = 20.0f,.y = 80.0f };
	for (uint32_t i = 0u; i < PLAYER_HP_MAX_QUANTITY_; ++i) {
		playerHP_[i].reset(Sprite::Create(playerHPTextureHandle, {.x=static_cast<float>(i)*64+ INITIAL_POSITION.x,.y= INITIAL_POSITION .y}));
	}
	
	playerHPBackFrame_.reset(Sprite::Create(playerHPBackFrameTextureHandle, { .x = INITIAL_POSITION.x,.y = INITIAL_POSITION.y }));
	currentDisplayHP_ = PLAYER_HP_MAX_QUANTITY_;


	//ゴールに向かえのテキスト
	uint32_t toEscapeTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Escape/ToGoal.png");
	toEscape_.reset(Sprite::Create(toEscapeTextureHandle, { .x = 0.0f,.y = 0.0f }));

#pragma endregion




	isGamePlay_ = false;
	isExplain_ = false;
	

	collisionManager_ = std::make_unique<CollisionManager>();
	
	theta_ = std::numbers::pi_v<float> / 2.0f;
	
	//ポストエフェクトの初期化
	back_ = std::make_unique< BackText>();
	back_->Initialize();
	//ビネット
	vignette_ = std::make_unique<Vignette>();
	vignette_->Initialize();
	//const float vignetteScale = 17.0f;
	vignettePow_ = 0.0f;
	//vignette_->SetScale(vignetteScale);
	vignette_->SetPow(vignettePow_);

	//マテリアルの初期化
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



void GameScene::KeyCollision(){

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

				//取得可能
				key->SetIsPrePickUp(true);

				//SPACEキーで取得
				if (input_->IsPushKey(DIK_SPACE) == true) {
					//プレイヤーの持っているか鍵の数が増える
					player_->AddHaveKeyQuantity();
					//鍵が取得される
					key->PickedUp();
				}

				//Bボタンを押したとき
				if (input_->IsConnetGamePad() == true){
					if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
						bTriggerTime_ += 1;

					}
					if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
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
	//trueの時に取得するかどうかのUIが出る
	isAbleToPickUpKey_ = std::any_of(keyes.begin(), keyes.end(), [](Key* key) {
		return key->GetIsPrePickUp() == true;
	});


#ifdef _DEBUG
	ImGui::Begin("KeyPickUp"); 
	ImGui::Checkbox("IsPickUp", &isAbleToPickUpKey_);
	ImGui::End();
#endif // _DEBUG


}

void GameScene::ObjectCollision(){
	
	//プレイヤーの移動方向
	Vector3 direction = playerMoveDirection_;
	//プレイヤーの当たり判定AABB
	AABB playerAABB = player_->GetAABB();


	//デモ用
	std::list <StageObjectPre*> stageObjects = objectManager_->GetStageObjets();
	for (StageObjectPre* stageObject : stageObjects) {
		
		//オブジェクトのAABB
		AABB objectAABB = stageObject->GetAABB();

		//オブジェクトとの差分ベクトル
		Vector3 objectAndPlayerDifference = VectorCalculation::Subtract(stageObject->GetWorldPosition(), playerPosition_);
		
		//オブジェクトとプレイヤーの距離
		Vector3 normalizedDemoAndPlayer = VectorCalculation::Normalize(objectAndPlayerDifference);
		//内積
		float dot  = SingleCalculation::Dot(direction, normalizedDemoAndPlayer);

		//衝突判定
		//だいたい内積は0.7くらいが良さそう
		if ((playerAABB.min.x <= objectAABB.max.x && playerAABB.max.x >= objectAABB.min.x) &&
			(playerAABB.min.z <= objectAABB.max.z && playerAABB.max.z >= objectAABB.min.z)&&
			(dot > 0.7f)) {
			uint32_t newCondition = PlayerMoveCondition::NonePlayerMove;
			player_->SetPlayerMoveCondition(newCondition);
			//当たったらループを抜ける
			break;

		}
		else {
			//当たっていない
			uint32_t newCondition = PlayerMoveCondition::OnPlayerMove;
			player_->SetPlayerMoveCondition(newCondition);

		}

	}
	

	
}

void GameScene::EscapeCondition(){
	//ゲート
	if (gate_->isCollision(playerPosition_)) {
#ifdef _DEBUG
		ImGui::Begin("InSpaceGate");
		ImGui::End();

#endif // _DEBUG

		//3個取得したら脱出できる
		uint32_t playerKeyQuantity = player_->GetHavingKey();
		if (playerKeyQuantity >= keyManager_->GetMaxKeyQuantity()) {
			//「脱出せよ」が表示
			escapeText_->SetInvisible(false);



			//コントローラーのBボタンを押したら脱出のフラグがたつ
			if (Input::GetInstance()->IsConnetGamePad() == true) {

				//Bボタンを押したとき
				if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += 1;

				}
				if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
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
		//まだ脱出できない
		escapeText_->SetInvisible(true);
	}

	//脱出
	if (isEscape_ == true) {
		isFadeOut_ = true;
	}

}

void GameScene::PlayerMove(){

	//何も押していないの時つまり動いていないので
	//通常はfalseにしておく
	isPlayerMoveKey_ = false;
	isPlayerMove_ = false;
	playerMoveDirection_ = { 0.0f,0.0f,0.0f };


	#pragma region キーボード
	//移動
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		playerMoveDirection_.x = std::cosf(theta_ - std::numbers::pi_v<float> / 2.0f);
		playerMoveDirection_.z = std::sinf(theta_ - std::numbers::pi_v<float> / 2.0f);
		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;

	}
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		playerMoveDirection_.x = std::cosf(theta_ + std::numbers::pi_v<float> / 2.0f);
		playerMoveDirection_.z = std::sinf(theta_ + std::numbers::pi_v<float> / 2.0f);

		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;
	}
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		playerMoveDirection_.x = std::cosf(theta_);
		playerMoveDirection_.z = std::sinf(theta_);

		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		playerMoveDirection_.x = std::cosf(theta_ + std::numbers::pi_v<float>);
		playerMoveDirection_.z = std::sinf(theta_ + std::numbers::pi_v<float>);

		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;
	}


	#pragma endregion

	#pragma region コントローラー

	//接続時
	if (Input::GetInstance()->IsConnetGamePad() == true) {
		//キーボード入力していない時・移動できる時に受け付ける
		if (isPlayerMoveKey_ == false) {


			//コントローラーの入力
			bool isInput = false;
			Vector3 leftStickInput = {
				.x = (static_cast<float>(input_->GetState().Gamepad.sThumbLX) / SHRT_MAX * 1.0f),
				.z = (static_cast<float>(input_->GetState().Gamepad.sThumbLY) / SHRT_MAX * 1.0f),
			};


			//デッドゾーン
			const float DEAD_ZONE = 0.1f;
			if (std::abs(leftStickInput.x) < DEAD_ZONE) {
				leftStickInput.x = 0.0f;
			}
			else {
				isInput = true;
			}
			if (std::abs(leftStickInput.z) < DEAD_ZONE) {
				leftStickInput.z = 0.0f;
			}
			else {
				isInput = true;
			}
		

			//入力されていたら計算
			if (isInput == true) {
				//動いている
				isPlayerMove_ = true;

				//角度を求める
				float radian = std::atan2f(leftStickInput.z, leftStickInput.x);
				//値を0～2πに直してtheta_に揃える
				if (radian < 0.0f) {
					radian += 2.0f * std::numbers::pi_v<float>;
				}
				const float OFFSET = std::numbers::pi_v<float> / 2.0f;
				float resultTheta = theta_ + radian - OFFSET;


				//向きを代入
				playerMoveDirection_.x = std::cosf(resultTheta);
				playerMoveDirection_.z = std::sinf(resultTheta);
			}



		}
	}

#pragma endregion


	//プレイヤーが動いている時
	if (isPlayerMove_ == true) {
		//状態の設定
		uint32_t newCondition = PlayerMoveCondition::OnPlayerMove;
		player_->SetPlayerMoveCondition(newCondition);

		//ダッシュ
		if (isPlayerMoveKey_ == true) {
			if (Input::GetInstance()->IsPushKey(DIK_RSHIFT) == true) {
				isPlayerDash_ = true;
			}
			else {
				isPlayerDash_ = false;
			}
		}
		//コントローラー接続時
		else {
			if (Input::GetInstance()->IsConnetGamePad() == true) {
				if (Input::GetInstance()->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) == true) {
					isPlayerDash_ = true;
				}
				else {
					isPlayerDash_ = false;
				}

			}
		}
		
		


	}
	//動いていない時
	else {
		uint32_t newCondition = PlayerMoveCondition::NonePlayerMove;
		player_->SetPlayerMoveCondition(newCondition);
	}

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::InputFloat3("Direction", &playerMoveDirection_.x);
	ImGui::Checkbox("IsPlayerMove", &isPlayerMove_);
	ImGui::End();

#endif // _DEBUG

	//プレイヤーの動く方向を入れる
	player_->SetMoveDirection(playerMoveDirection_);

	//ダッシュをしているかどうかの設定
	player_->SetIsDash(isPlayerDash_);

}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update(GameManager* gameManager) {

	//フレーム初めに
	//コリジョンリストのクリア
	collisionManager_->ClearList();
	//フェードの透明度の設定
	fadeSprite_->SetTransparency(fadeTransparency_);


	//StatePatternにしたい
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
	//StatePatternにしたい
	if (isFadeIn == false && isFadeOut_ == false) {
		fadeTransparency_ = 0.0f;
		
		if (isExplain_ == true) {
			if (input_->IsTriggerKey(DIK_SPACE) == true) {
				++howToPlayTextureNumber_;
			}
			//Bボタンを押したとき
			if (input_->IsConnetGamePad() == true) {
				if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += 1;

				}
				if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
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
		if (Input::GetInstance()->IsConnetGamePad() == true) {
			const float MOVE_LIMITATION = 0.02f;

			//キーボード入力していない時
			if (isRotateYKey_ == false && isRotateXKey_ == false) {

				//入力
				float rotateMoveX = (float)input_->GetState().Gamepad.sThumbRY / SHRT_MAX * ROTATE_OFFSET;
				float rotateMoveY = (float)input_->GetState().Gamepad.sThumbRX / SHRT_MAX * ROTATE_OFFSET;
				
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

		#pragma endregion


		//プレイヤーの移動
		PlayerMove();



		//数学とプログラムで回る向きが違うことに煩わしさを感じます・・
		float phi = -originPhi_;

		//懐中電灯
		playerPosition_ = player_->GetWorldPosition();

		//ライトはプレイヤーが持っているという包含の関係なのでPlayerに入れた方が良いかも。
		//ここでやるべきではないと思う。
		flashLight_->SetPlayerPosition(playerPosition_);
		flashLight_->SetTheta(theta_);
		flashLight_->SetPhi(phi);
		flashLight_->Update();

		


		//エネミーをコリジョンマネージャーに追加
		std::list<Enemy*> enemyes = enemyManager_->GetEnemyes();
		for (Enemy* enemy : enemyes) {
			collisionManager_->RegisterList(enemy);
			collisionManager_->RegisterList(enemy->GetEnemyFlashLightCollision());

			//攻撃用の判定が出ていたら登録
			if (enemy->GetIsAttack() == true) {
				collisionManager_->RegisterList(enemy->GetEnemyAttackCollision());
				collisionManager_->RegisterList(player_->GetCollisionToNormalEnemy());


			}
		}

		
		//当たると一発アウトの敵をコリジョンマネージャーへ
		//1体しか出さないのにリストにする必要はあったのでしょうか・・
		collisionManager_->RegisterList(player_->GetCollisionToStrongEnemy());
		std::list<StrongEnemy*> strongEnemyes = enemyManager_->GetStrongEnemyes();
		for (StrongEnemy* strongEnemy : strongEnemyes) {
			collisionManager_->RegisterList(strongEnemy);
			bool isTouch = strongEnemy->GetIsTouchPlayer();

			if (isTouch == true) {
				isTouchStrongEnemy_ = true;
			}
		}



		

		//もとに戻す
		camera_.rotate_.x = -phi;
		camera_.rotate_.y = -(theta_)+std::numbers::pi_v<float> / 2.0f;
		camera_.rotate_.z = 0.0f;

		camera_.translate_ = VectorCalculation::Add(playerPosition_, CAMERA_POSITION_OFFSET);




		#pragma region 鍵の取得処理
		keyQuantity_ = keyManager_->GetKeyQuantity();
		//鍵が0より多ければ通る
		if (keyQuantity_ > 0) {
			KeyCollision();
		}
		else {
			isAbleToPickUpKey_ = false;
		}

		#pragma endregion

		//脱出の仕組み
		EscapeCondition();
		
		
		
		//鍵
		keyManager_->Update();

		//オブジェクトの当たり判定
		ObjectCollision();



		//体力が0になったら負け
		//または一発アウトの敵に接触した場合
		//負け専用のクラスを作りたい
		if (player_->GetHP() <= 0 || isTouchStrongEnemy_==true) {
			gameManager->ChangeScene(new LoseScene());
			return;
		}


		//ライト
		Vector3 lightDirection = flashLight_->GetDirection();

		//現在のプレイヤーの体力を取得
		currentDisplayHP_ = player_->GetHP();
		

		//更新
		material_.Update();

		//当たり判定チェック
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
	//StatePatternにしたい
	if (isFadeOut_ == true) {
		escapeText_->SetInvisible(true);

		player_->SetIsAbleToControll(false);
		const float FADE_OUT_INTERVAL = 0.01f;
		fadeTransparency_ += FADE_OUT_INTERVAL;
		fadeSprite_->SetTransparency(fadeTransparency_);

		if (fadeTransparency_ > 1.0f) {
			gameManager->ChangeScene(new LoseScene());
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
	playerPosition_ = player_->GetWorldPosition();


#pragma region ポストエフェクト
	//プレイヤーがダメージを受けた場合ビネット
	if (player_->GetIsDamaged() == true) {
		//時間の加算
		const float DELTA_TIME = 1.0f / 60.0f;
		vignetteChangeTime_ += DELTA_TIME;
		
		//線形補間で滑らかに変化
		vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, vignetteChangeTime_);
	}
	//HPが1でピンチの場合
	else if (player_->GetHP() == 1u) {
		
		const float DELTA_TIME = 1.0f / 60.0f;
		warningTime_ += DELTA_TIME;
		vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, warningTime_);
		if (warningTime_ > 1.0f) {
			warningTime_ = 0.0f;
		}

	}
	//通常時の場合
	else {
		vignettePow_ = 0.0f;
		vignetteChangeTime_ = 0.0f;
	}
	vignette_->SetPow(vignettePow_);

#ifdef _DEBUG
	ImGui::Begin("VignetteCheck");
	ImGui::InputFloat("POW", &vignettePow_);
	ImGui::InputFloat("変化の時間", &vignetteChangeTime_);
	ImGui::End();
#endif // _DEBUG


#pragma endregion

	//ライト確認用のタワー
	debugTowerWorldTransform_.Update();

	debugFanCollisionSphereWorldTransform_.Update();
	debugFanCollisionSphereMaterial_.Update();

}

void GameScene::DrawSpriteBack(){

}

void GameScene::PreDrawPostEffectFirst(){
	vignette_->PreDraw();
}


void GameScene::DrawObject3D() {
	

	//懐中電灯を取得
	SpotLight spotLight = flashLight_->GetSpotLight();

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

	//鍵
	keyManager_->DrawObject3D(camera_, spotLight);


	

}



void GameScene::DrawPostEffect(){
	vignette_->Draw();
}

void GameScene::DrawSprite(){
	
	//説明
	if (howToPlayTextureNumber_ == 1u) {
		explanation_[0]->Draw();
		spaceToNext_[0]->Draw();
	}
	if (howToPlayTextureNumber_ == 2u) {
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

		//プレイヤーの体力の枠
		playerHPBackFrame_->Draw();

		//プレイヤーの体力(アイコン型)
		for (uint32_t i = 0u; i < currentDisplayHP_; ++i) {
			playerHP_[i]->Draw();
		}
		if (player_->GetHavingKey() == keyManager_->GetMaxKeyQuantity()) {
			toEscape_->Draw();
		}

		

	}

	//フェード
	if (isFadeIn == true || isFadeOut_ == true) {
		fadeSprite_->Draw();
	}


}



GameScene::~GameScene() {
	delete objectManager_;
}


