#include "GameScene.h"
#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"

#include "SingleCalculation.h"
#include "VectorCalculation.h"


#include "AnimationManager.h"
#include "TextureManager.h"
#include "GameManager.h"
#include "ModelManager.h"


GameScene::GameScene(){

	//入力
	input_ = Input::GetInstance();

	//テクスチャ管理クラス
	texturemanager_ = TextureManager::GetInstance();

	//モデル管理クラス
	modelManager_ = ModelManager::GetInstance();

	//レベルエディタ管理クラスの取得
	levelDataManager_ = LevelDataManager::GetInstance();
}

void GameScene::Initialize() {

#pragma region フェード
	
	//白フェード
	//白画像読み込み
	uint32_t whiteTextureHandle = texturemanager_->LoadTexture("Resources/Sprite/Back/White.png");
	//フェードの初期座標
	const Vector2 INITIAL_FADE_POSITION = { .x = 0.0f,.y = 0.0f };
	//生成
	whiteFade_.reset(Sprite::Create(whiteTextureHandle, INITIAL_FADE_POSITION));
	
	//フェードインから始まる
	//イン
	isWhiteFadeIn = true;
	//アウト
	isWhiteFadeOut_ = false;
	//イン
	isBlackFadeIn = false;
	//アウト
	isBlackFadeOut_ = false;


	//黒フェード
	//黒画像読み込み
	uint32_t blackTextureHandle = texturemanager_->LoadTexture("Resources/Sprite/Back/Black.png");
	//生成
	blackFade_.reset(Sprite::Create(blackTextureHandle, INITIAL_FADE_POSITION));
	
	//透明度
	blackFadeTransparency_ = 0.0f;
	


#ifdef _DEBUG
	isWhiteFadeIn = false;
	isWhiteFadeOut_ = false;

#endif // _DEBUG

#pragma endregion


	
#pragma region オブジェクト
	//生成
	objectManager_ = std::make_unique<ObjectManager>();
	//初期化
	objectManager_->Initialize();

	#pragma region 地面
	//四隅の座標を取得
	StageRect stageRect = {};
	//四隅
	const float SCALE_SIZE_ = 40.0f;
	stageRect.leftBack = { .x = -SCALE_SIZE_ ,.y = 0.0f ,.z = SCALE_SIZE_ };
	stageRect.rightBack = { .x = SCALE_SIZE_ ,.y = 0.0f ,.z = SCALE_SIZE_ };
	stageRect.leftFront = { .x = -SCALE_SIZE_ ,.y = 0.0f ,.z = -SCALE_SIZE_ };
	stageRect.rightFront = { .x = SCALE_SIZE_ ,.y = 0.0f ,.z = -SCALE_SIZE_ };


	#pragma endregion

	
	#pragma region ゲート

	//ゲートのモデルの読み込み
	uint32_t gateModelhandle = modelManager_->LoadModelFile("Resources/Model/Sample/Gate","Gate.obj");
	//生成
	gate_ = std::make_unique<Gate>();
	//初期化
	gate_->Initialize(gateModelhandle);



	//「脱出せよ」の画像読み込み
	uint32_t escapeTextureHandle = texturemanager_->LoadTexture("Resources/Game/Escape/EscapeText.png");
	//生成
	escapeText_.reset(Sprite::Create(escapeTextureHandle, { .x = 0.0f,.y = 0.0f }));
	//最初は非表示にする
	escapeText_->SetInvisible(true);
	#pragma endregion

#pragma endregion

#pragma region プレイヤー
	//生成
	player_ = std::make_unique<Player>();
	//初期化
	player_->Initialize();
	//最初はコントロールは出来ない用にする
	player_->SetIsAbleToControll(false);

	//向きの初期化
	playerMoveDirection_ = {.x = 0.0f,.y = 0.0f,.z = 0.0f };
	//気ボードは何も押していない
	isPlayerMoveKey_ = false;
	//Bトリガーの時間を初期化
	bTriggerTime_ = 0;
	//Bトリガーの初期化
	isBTrigger_ = false;

	



#pragma endregion

#pragma region 鍵
	//鍵のモデルの読み込み
	uint32_t keyModelHandle = modelManager_->LoadModelFile("Resources/External/Model/key","Key.obj");
	//生成
	keyManager_ = std::make_unique<KeyManager>();
	//プレイヤーの設定
	keyManager_->SetPlayer(player_.get());
	//初期化
	keyManager_->Initialize(keyModelHandle);
	
#pragma endregion
	
	//天球
	//モデル読み込み
	uint32_t skydomeModelHandle = modelManager_->LoadModelFile("Resources/Game/Skydome","Skydome.obj");
	//生成
	skydome_ = std::make_unique<Skydome>();
	//初期化
	skydome_->Initialize(skydomeModelHandle);

	//ハンドルの取得
	levelHandle_=levelDataManager_->Load("GameStage/GameStage.json");



#pragma region 敵
	//敵モデルの読み込み
	//通常
	uint32_t enemyModelHandle = modelManager_->LoadModelFile("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "Ghost.gltf");
	//強敵
	uint32_t strongEnemyModelHandle= modelManager_->LoadModelFile("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "StrongGhost.gltf");

#ifdef _DEBUG
	enemyModelHandle = modelManager_->LoadModelFile("Resources/Model/Sample/Cube", "Cube.obj");
#endif // _DEBUG

	//敵管理システム
	enemyManager_ = std::make_unique<EnemyManager>();
	//プレイヤーの設定
	enemyManager_->SetPlayer(player_.get());
	//オブジェクト管理クラスの設定
	enemyManager_->SetObjectManager(objectManager_.get());
	//レベルデータ管理クラスの設定
	enemyManager_->SetLevelDataManager(levelDataManager_, levelHandle_);
	//ステージの四隅の情報を設定
	enemyManager_->SetStageRectangle(stageRect);
	//初期化
	enemyManager_->Initialize(enemyModelHandle, strongEnemyModelHandle);
	#pragma endregion

	#pragma region カメラ

	//カメラの初期化
	camera_.Initialize();
	//それぞれに値を入れていく
	//回転
	//+で左回り
	camera_.rotate_.y = std::numbers::pi_v<float> / 2.0f;
	//座標
	camera_.translate_ = { .x = 0.0f,.y = 1.0f ,.z = -15.0f };
	

	//カメラ座標のオフセットの初期化
	cameraPositionOffset_ = { .x = 0.0f,.y = 2.0f,.z = 0.0f };
	
	#pragma endregion
	
	#pragma region 説明

	//説明画像の読み込み
	uint32_t explanationTextureHandle[EXPLANATION_QUANTITY_] = {};
	explanationTextureHandle[0] = texturemanager_->LoadTexture("Resources/Game/Explanation/Explanation1.png");
	explanationTextureHandle[1] = texturemanager_->LoadTexture("Resources/Game/Explanation/Explanation2.png");

	//生成
	for (uint32_t i = 0u; i < EXPLANATION_QUANTITY_; ++i) {
		explanation_[i].reset(Sprite::Create(explanationTextureHandle[i], INITIAL_FADE_POSITION));
	}
	
	//スペースで次への画像読み込み
	uint32_t spaceToNextTextureHandle[SPACE_TO_NEXT_QUANTITY_] = {};
	spaceToNextTextureHandle[0] = texturemanager_->LoadTexture("Resources/Game/Explanation/ExplanationNext1.png");
	spaceToNextTextureHandle[1] = texturemanager_->LoadTexture("Resources/Game/Explanation/ExplanationNext2.png");

	for (uint32_t i = 0; i < SPACE_TO_NEXT_QUANTITY_; ++i) {
		spaceToNext_[i].reset(Sprite::Create(spaceToNextTextureHandle[i], INITIAL_FADE_POSITION));
	}

	//最初は0番目
	howToPlayTextureNumber_ = 0u;

	//常時表示
	//操作
	uint32_t operationTextureHandle = texturemanager_->LoadTexture("Resources/Game/Operation/Operation.png");
	//生成
	operation_.reset(Sprite::Create(operationTextureHandle, {.x=20.0f,.y=0.0f}));

	//拾う画像の読み込み
	uint32_t pickUpTextureManager = texturemanager_->LoadTexture("Resources/Game/Key/PickUpKey.png");
	//生成
	pickUpKey_.reset(Sprite::Create(pickUpTextureManager, INITIAL_FADE_POSITION));
	
#pragma endregion

	#pragma region UI
	uint32_t playerHPTextureHandle = texturemanager_->LoadTexture("Resources/Player/PlayerHP.png");
	uint32_t playerHPBackFrameTextureHandle = texturemanager_->LoadTexture("Resources/Player/PlayerHPBack.png");
	const Vector2 INITIAL_POSITION = { .x = 20.0f,.y = 80.0f };
	for (uint32_t i = 0u; i < PLAYER_HP_MAX_QUANTITY_; ++i) {
		playerHP_[i].reset(Sprite::Create(playerHPTextureHandle, {.x=static_cast<float>(i)*64+ INITIAL_POSITION.x,.y= INITIAL_POSITION .y}));
	}
	
	playerHPBackFrame_.reset(Sprite::Create(playerHPBackFrameTextureHandle, { .x = INITIAL_POSITION.x,.y = INITIAL_POSITION.y }));
	currentDisplayHP_ = PLAYER_HP_MAX_QUANTITY_;


	//ゴールに向かえのテキスト
	uint32_t toEscapeTextureHandle = texturemanager_->LoadTexture("Resources/Game/Escape/ToGoal.png");
	toEscape_.reset(Sprite::Create(toEscapeTextureHandle, INITIAL_FADE_POSITION));

	#pragma endregion




	
	
	//コリジョン管理クラスの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	//角度の初期化
	//プレイヤーの向いている向きと合わせていくよ
	theta_ = std::numbers::pi_v<float> / 2.0f;
	
	//ポストエフェクトの初期化
	//基本
	//生成
	back_ = std::make_unique<BackText>();
	//初期化
	back_->Initialize();
	//ビネット
	//生成
	vignette_ = std::make_unique<Vignette>();
	//初期化
	vignette_->Initialize();
	//値の設定
	vignettePow_ = 0.0f;
	vignette_->SetPow(vignettePow_);

	//マテリアルの初期化
	material_.Initialize();
	//ライティングの種類を設定
	material_.lightingKinds_ = Spot;
	


	//シーンのどこから始めるかを設定する
	//StatePatternで分かりやすくしたい
	isGamePlay_ = false;
	isExplain_ = false;


#ifdef _DEBUG 
	//デバッグするときはこっちで設定
	isWhiteFadeIn = false;
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
			Vector3 distance = {
				.x = std::powf((player_->GetWorldPosition().x - key->GetWorldPosition().x), 2.0f),
				.y = 0.0f,
				.z = std::powf((player_->GetWorldPosition().z - key->GetWorldPosition().z), 2.0f),
			};

			//距離
			float collisionDistance = sqrtf(distance.x + distance.y + distance.z);
			


			//範囲内にいれば入力を受け付ける
			if (collisionDistance <= player_->GetSideSize() + key->GetRadius()) {

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
						
						bTriggerTime_ += INCREASE_VALUE;

					}
					if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == NO_PUSH_VALUE_) {
						bTriggerTime_ = B_NO_REACT_TIME_;
					}

					if (bTriggerTime_ == B_REACT_TIME_) {
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
	
	//ただ衝突判定を設定するだけだと出来なかったので
	//内積の計算も入れて可能にする


	//プレイヤーの移動方向
	Vector3 direction = playerMoveDirection_;
	//プレイヤーの当たり判定AABB
	AABB playerAABB = player_->GetAABB();




	//座標
	std::vector<Vector3> positions = levelDataManager_->GetStageObjectPositions(levelHandle_);
	//AABB
	std::vector<AABB> aabbs = levelDataManager_->GetStageObjectAABBs(levelHandle_);
	//コライダーを持っているかどうか
	std::vector<bool> colliders = levelDataManager_->GetIsHavingColliders(levelHandle_);
	//衝突判定
	for (size_t i = 0; i < positions.size() && i < aabbs.size()&&i<colliders.size(); ++i) {
		
		//コライダーを持っているときだけ
		if (colliders[i] == true) {
			//オブジェクトとの差分ベクトル
			Vector3 objectAndPlayerDifference = VectorCalculation::Subtract(positions[i], player_->GetWorldPosition());

			//オブジェクトとプレイヤーの距離
			Vector3 normalizedDemoAndPlayer = VectorCalculation::Normalize(objectAndPlayerDifference);

			//内積
			//これが無いと接触したまま動けなくなってしまうので入れる
			float dot = SingleCalculation::Dot(direction, normalizedDemoAndPlayer);
			const float DOT_OFFSET = 0.7f;


			//衝突判定
			//Y成分はいらない
			if ((playerAABB.min.x <= aabbs[i].max.x && playerAABB.max.x >= aabbs[i].min.x) &&
				(playerAABB.min.z <= aabbs[i].max.z && playerAABB.max.z >= aabbs[i].min.z) &&
				(dot > DOT_OFFSET)) {
				uint32_t newCondition = PlayerMoveCondition::NonePlayerMove;
				player_->SetMoveCondition(newCondition);

				//当たったらループを抜ける
				break;
			}
			else {
				//当たっていない
				uint32_t newCondition = PlayerMoveCondition::OnPlayerMove;
				player_->SetMoveCondition(newCondition);

			}
		}

		

	}
	
}

void GameScene::EscapeCondition(){
	//ゲート
	if (gate_->isCollision(player_->GetWorldPosition())) {
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
			if (input_->IsConnetGamePad() == true) {

				//Bボタンを押したとき
				if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += INCREASE_VALUE;

				}
				if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == NO_PUSH_VALUE_) {
					bTriggerTime_ = B_NO_REACT_TIME_;
				}

				if (bTriggerTime_ == B_REACT_TIME_) {
					//脱出
					isEscape_ = true;
				}

			}
			//SPACEキーを押したら脱出のフラグがたつ
			if (input_->IsPushKey(DIK_SPACE) == true) {
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
		isWhiteFadeOut_ = true;
	}

}

void GameScene::PlayerMove(){

	//何も押していない時つまり動いていないので
	//通常はfalseと0にしておく
	isPlayerMoveKey_ = false;
	isPlayerMove_ = false;
	playerMoveDirection_ = { .x = 0.0f,.y = 0.0f,.z = 0.0f };


	#pragma region キーボード
	//移動
	//Dキー(右)
	if (input_->IsPushKey(DIK_D) == true) {
		//動く方向
		playerMoveDirection_ = {
			.x = std::cosf(theta_ - std::numbers::pi_v<float> / 2.0f),
			.y = 0.0f,
			.z = std::sinf(theta_ - std::numbers::pi_v<float> / 2.0f),
		};
		
		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;

	}
	//Aキー(左)
	if (input_->IsPushKey(DIK_A) == true) {
		//動く方向
		playerMoveDirection_ = {
			.x = std::cosf(theta_ + std::numbers::pi_v<float> / 2.0f),
			.y = 0.0f,
			.z = std::sinf(theta_ + std::numbers::pi_v<float> / 2.0f),
		};

		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;
	}
	//Wキー(前)
	if (input_->IsPushKey(DIK_W) == true) {
		//動く方向
		playerMoveDirection_ = {
			.x = std::cosf(theta_),
			.y = 0.0f,
			.z = std::sinf(theta_),
		};
		
		//キーボード入力をしている
		isPlayerMoveKey_ = true;
		//動いている
		isPlayerMove_ = true;
	}
	//Sキー(後ろ)
	if (input_->IsPushKey(DIK_S) == true) {
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
	//キーボード入力していない時かつ移動できる時に受け付ける
	if (input_->IsConnetGamePad() == true) {
		
		if (isPlayerMoveKey_ == false) {


			//コントローラーの入力
			bool isInput = false;
			//左スティック
			Vector3 leftStickInput = {
				.x = (static_cast<float>(input_->GetState().Gamepad.sThumbLX) / SHRT_MAX * 1.0f),
				.y = 0.0f,
				.z = (static_cast<float>(input_->GetState().Gamepad.sThumbLY) / SHRT_MAX * 1.0f),
			};


			//デッドゾーンの設定
			const float DEAD_ZONE = 0.1f;
			//X軸
			if (std::abs(leftStickInput.x) < DEAD_ZONE) {
				leftStickInput.x = 0.0f;
			}
			else {
				isInput = true;
			}
			//Z軸
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
		player_->SetMoveCondition(newCondition);

		//ダッシュ
		if (isPlayerMoveKey_ == true) {
			if (input_->IsPushKey(DIK_RSHIFT) == true) {
				isPlayerDash_ = true;
			}
			else {
				isPlayerDash_ = false;
			}
		}
		//コントローラー接続時
		else {
			if (input_->IsConnetGamePad() == true) {
				if (input_->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) == true) {
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
		player_->SetMoveCondition(newCondition);
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


void GameScene::Update(GameManager* gameManager) {

	//フレーム初めに
	//コリジョンリストのクリア
	collisionManager_->ClearList();
	//フェードの透明度の設定
	whiteFade_->SetTransparency(whiteFadeTransparency_);

	

	//StatePatternにしたい
	//フェードイン
	if (isWhiteFadeIn==true) {
		const float FADE_IN_INTERVAL = 0.01f;
		whiteFadeTransparency_ -= FADE_IN_INTERVAL;
		
		//完全に透明になったらゲームが始まる
		if (whiteFadeTransparency_ < PERFECT_TRANSPARENT_) {
			whiteFadeTransparency_ = PERFECT_TRANSPARENT_;
			isWhiteFadeIn = false;
			isExplain_ = true;
			//1枚目
			howToPlayTextureNumber_ = 1;
		}
	}

	//ゲーム
	//StatePatternにしたい
	if (isWhiteFadeIn == false && isWhiteFadeOut_ == false) {
		whiteFadeTransparency_ = PERFECT_TRANSPARENT_;
		
		//説明
		if (isExplain_ == true) {
			if (input_->IsTriggerKey(DIK_SPACE) == true) {
				++howToPlayTextureNumber_;
			}
			
			//コントローラー接続時
			if (input_->IsConnetGamePad() == true) {
				//Bボタンを押したとき
				if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += INCREASE_VALUE;

				}
				//押していない時
				if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == NO_PUSH_VALUE_) {
					bTriggerTime_ = B_NO_REACT_TIME_;
				}

				//1の時テクスチャの数字が増える
				if (bTriggerTime_ == B_REACT_TIME_) {
					++howToPlayTextureNumber_;
				}
			}

			//読み終わったらゲームプレイへ
			if (howToPlayTextureNumber_ > MAX_EXPLANATION_NUMBER_) {
				isExplain_ = false;
				isGamePlay_ = true;
			}
		}
		if (isGamePlay_ == true) {
			//コントロール可能にする
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
		
		//左を向く
		if (input_->IsPushKey(DIK_LEFT) == true) {
			theta_ += ROTATE_INTERVAL;
			isRotateYKey_ = true;
		}
		//右を向く
		if (input_->IsPushKey(DIK_RIGHT) == true) {
			theta_ -= ROTATE_INTERVAL;
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
		if (input_->IsPushKey(DIK_UP) == true) {
			originPhi_ -= ROTATE_INTERVAL;
			isRotateXKey_ = true;
		}
		//下を向く
		if (input_->IsPushKey(DIK_DOWN) == true) {
			originPhi_ += ROTATE_INTERVAL;
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
		if (input_->IsConnetGamePad() == true) {
			const float MOVE_LIMITATION = 0.02f;

			//キーボード入力していない時
			if (isRotateYKey_ == false && isRotateXKey_ == false) {

				//入力
				float rotateMoveX = (float)input_->GetState().Gamepad.sThumbRY / SHRT_MAX * ROTATE_INTERVAL;
				float rotateMoveY = (float)input_->GetState().Gamepad.sThumbRX / SHRT_MAX * ROTATE_INTERVAL;
				
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



		//数学とプログラムで回る向きが違うことに煩わしさを感じるので
		//無理矢理直す
		float phi = -originPhi_;

		//プレイヤーにそれぞれの角度を設定する
		player_->GetFlashLight()->SetTheta(theta_);
		player_->GetFlashLight()->SetPhi(phi);
		


		//エネミーをコリジョンマネージャーに追加
		//通常の敵のリストの取得
		std::list<Enemy*> enemyes = enemyManager_->GetEnemyes();
		for (Enemy* enemy : enemyes) {
			//懐中電灯に対して
			collisionManager_->RegisterList(enemy->GetEnemyFlashLightCollision());
			
			//攻撃
			if (enemy->GetIsAttack() == true) {
				player_->SetIsAcceptDamegeFromNoemalEnemy(true);
			}
			else {
				player_->SetIsAcceptDamegeFromNoemalEnemy(false);
			}
			
			//敵の攻撃
			collisionManager_->RegisterList(enemy->GetEnemyAttackCollision());
			
		}
		//通常の敵に対してのコライダーを登録
		collisionManager_->RegisterList(player_->GetCollisionToNormalEnemy());
		//オーディオオブジェクトに対してのコライダーを登録
		collisionManager_->RegisterList(player_->GetCollisionToAudioObject());
		//懐中電灯に対してのコライダーを登録
		//collisionManager_->RegisterList(player_->GetFlashLightCollision());
		//当たると一発アウトの敵をコリジョンマネージャーへ
		collisionManager_->RegisterList(player_->GetCollisionToStrongEnemy());
		std::list<StrongEnemy*> strongEnemyes = enemyManager_->GetStrongEnemyes();
		for (StrongEnemy* strongEnemy : strongEnemyes) {
			bool isTouch = strongEnemy->GetIsTouchPlayer();

			collisionManager_->RegisterList(strongEnemy);
			//接触
			if (isTouch == true) {

				isTouchStrongEnemy_ = true;
			}
		}



		//レベルエディタで使うリスナーの設定
		Listener listener = {
			.position = player_->GetWorldPosition(),
			.move = player_->GetDirection(),
		};
		levelDataManager_->SetListener(levelHandle_, listener);

		//レベルエディタの更新
		levelDataManager_->Update(levelHandle_);

		std::vector<IObjectForLevelEditorCollider*> audioColliders =levelDataManager_->GetAudioCollider(levelHandle_);

		for (std::vector<IObjectForLevelEditorCollider*>::iterator it = audioColliders.begin(); it != audioColliders.end(); ++it) {

			collisionManager_->RegisterList(*it);

		}


#ifdef _DEBUG
		if (input_->IsTriggerKey(DIK_R) == true) {
			levelDataManager_->Reload(levelHandle_);
		}
#endif // _DEBUG



		//当たり判定チェック
		collisionManager_->CheckAllCollision();


		//もとに戻す
		//カメラの回転の計算
		camera_.rotate_.x = -phi;
		camera_.rotate_.y = -(theta_)+std::numbers::pi_v<float> / 2.0f;
		camera_.rotate_.z = 0.0f;

		//位置の計算
		camera_.translate_ = VectorCalculation::Add(player_->GetWorldPosition(), cameraPositionOffset_);




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
		const uint32_t MIN_HP = 0u;
		if (player_->GetHP() <= MIN_HP || isTouchStrongEnemy_==true) {

			//敵の動きが止まりブラックアウト
			//プレイヤーことカメラが倒れる感じが良いかも
			isBlackFadeOut_ = true;

			blackFadeTransparency_ += FADE_OUT_INTERVAL_;
			blackFade_->SetTransparency(blackFadeTransparency_);


			//負けシーンへ
			if (blackFadeTransparency_ > CHANGE_TO_LOSE_SCENE_VALUE_) {
				gameManager->ChangeScene(new LoseScene());
				return;
			}
			
		}



		//現在のプレイヤーの体力を取得
		currentDisplayHP_ = player_->GetHP();
		
		
		

#ifdef _DEBUG
		ImGui::Begin("カメラ");
		ImGui::SliderFloat3("回転", &camera_.rotate_.x, -3.0f, 3.0f);
		ImGui::SliderFloat3("位置", &cameraTranslate.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("オフセット位置", &cameraPositionOffset_.x, -30.0f, 30.0f);
		ImGui::InputFloat("Theta", &theta_);
		ImGui::InputFloat("Phi", &phi);
		ImGui::End();
#endif

	}
	
	//ホワイトアウト
	//StatePatternにしたい
	if (isWhiteFadeOut_ == true) {
		escapeText_->SetInvisible(true);
		//振動しないようにする
		player_->SetIsAbleToControll(false);
		
		//加算
		whiteFadeTransparency_ += FADE_OUT_INTERVAL_;
		//透明度の設定
		whiteFade_->SetTransparency(whiteFadeTransparency_);

		//最大の透明度
		//本当は1.0fだけど新しく変数を作るとネストが増えるので一緒にやることにした。
		const float MAX_TRANSPARENCY = 2.0f;
		if (whiteFadeTransparency_ > MAX_TRANSPARENCY) {
			gameManager->ChangeScene(new WinScene());
			return;
		}
		
	}

	
	//カメラの更新
	camera_.Update();

	//更新
	material_.Update();



	//プレイヤーの更新
	player_->Update();
	

	//門
	gate_->Update();

	//天球
	skydome_->Update();

	

	
#pragma region ポストエフェクト
	//HPが1でピンチの場合
	const uint32_t DANGEROUS_HP = 1u;
	//プレイヤーがダメージを受けた場合ビネット
	if (player_->GetIsDamaged() == true) {
		//時間の加算
		vignetteChangeTime_ += DELTA_TIME_;
		
		//線形補間で滑らかに変化
		vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, vignetteChangeTime_);
	}
	
	else if (player_->GetHP() == DANGEROUS_HP) {
		warningTime_ += DELTA_TIME_;
		vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, warningTime_);

		//最大時間
		const float MAX_WARNING_TIME = 1.0f;
		//最小時間
		const float MIN_WARNING_TIME = 1.0f;

		if (warningTime_ > MAX_WARNING_TIME) {
			warningTime_ = MIN_WARNING_TIME;
		}
	}
	//通常時の場合
	else {
		vignettePow_ = 0.0f;
		vignetteChangeTime_ = 0.0f;
	}
	vignette_->SetPow(vignettePow_);

#ifdef _DEBUG
	ImGui::Begin("ビネットの確認");
	ImGui::InputFloat("POW", &vignettePow_);
	ImGui::InputFloat("変化の時間", &vignetteChangeTime_);
	ImGui::End();

	ImGui::Begin("フェード");
	ImGui::InputFloat("白", &whiteFadeTransparency_);
	ImGui::InputFloat("黒", &blackFadeTransparency_);
	ImGui::End();


#endif // _DEBUG




#pragma endregion

}


void GameScene::PreDrawPostEffectFirst(){
	//ビネット描画処理前
	vignette_->PreDraw();
}


void GameScene::DrawObject3D() {
	
	//懐中電灯を取得
	SpotLight spotLight = player_->GetFlashLight()->GetSpotLight();

	//ゲート
	gate_->Draw(camera_, spotLight);
	//敵
	enemyManager_->Draw(camera_, spotLight);
	//天球
	skydome_->Draw(camera_);

	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_, material_, spotLight);

	//プレイヤー
	player_->Draw(camera_, spotLight);
	//鍵
	keyManager_->DrawObject3D(camera_, spotLight);

}



void GameScene::DrawPostEffect(){
	//ビネット描画
	vignette_->Draw();
}

void GameScene::DrawSprite(){
	
	
	//最大数
	const uint32_t MAX_TEXTURE_QUANTITY = 2;
	//説明
	for (uint32_t i = 0; i < MAX_TEXTURE_QUANTITY; ++i) {
		if (howToPlayTextureNumber_ == i+1) {
			explanation_[i]->Draw();
			spaceToNext_[i]->Draw();
		}
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
	if (isWhiteFadeIn == true || isWhiteFadeOut_ == true) {
		whiteFade_->Draw();
	}
	//黒フェード
	if (isBlackFadeIn==true|| isBlackFadeOut_==true) {
		blackFade_->Draw();
	}


}


