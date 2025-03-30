#include "GameScene.h"
#include <imgui.h>
#include <numbers>
#include <algorithm>

#include "Input.h"
#include "SingleCalculation.h"
#include "VectorCalculation.h"
#include "PushBackCalculation.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "GameManager.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "GlobalVariables.h"

GameScene::GameScene() {
	//インスタンスの取得
	//入力
	input_ = Elysia::Input::GetInstance();
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
	//グローバル変数クラス
	globalVariables_ = Elysia::GlobalVariables::GetInstance();
}

void GameScene::Initialize() {

#pragma region フェード

	//白フェード
	//白画像読み込み
	uint32_t whiteTextureHandle = textureManager_->Load("Resources/Sprite/Back/White.png");
	//フェードの初期座標
	const Vector2 INITIAL_SPRITE_POSITION = { .x = 0.0f,.y = 0.0f };
	//生成
	whiteFade_.reset(Elysia::Sprite::Create(whiteTextureHandle, INITIAL_SPRITE_POSITION));

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
	uint32_t blackTextureHandle = textureManager_->Load("Resources/Sprite/Back/Black.png");
	//生成
	blackFade_.reset(Elysia::Sprite::Create(blackTextureHandle, INITIAL_SPRITE_POSITION));

	//透明度
	blackFadeTransparency_ = 0.0f;



#ifdef _DEBUG
	isWhiteFadeIn = false;
	isWhiteFadeOut_ = false;

#endif // _DEBUG

#pragma endregion

#pragma region ゲート

	//ゲートのモデルの読み込み
	uint32_t gateModelhandle = modelManager_->Load("Resources/Model/Sample/Gate", "Gate.obj");
	//生成
	gate_ = std::make_unique<Gate>();
	//初期化
	gate_->Initialize(gateModelhandle);

	//「脱出せよ」の画像読み込み
	uint32_t escapeTextureHandle = textureManager_->Load("Resources/Sprite/Escape/EscapeText.png");
	//生成
	escapeText_.reset(Elysia::Sprite::Create(escapeTextureHandle, { .x = 0.0f,.y = 0.0f }));
	//最初は非表示にする
	escapeText_->SetInvisible(true);
#pragma endregion

	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("GameStage/GameStage.json");

	//門の初期回転
	rightGateRotateTheta_ = 0.0f;
	leftGateRotateTheta_ = -std::numbers::pi_v<float_t>;

#pragma region プレイヤー
	//生成
	player_ = std::make_unique<Player>();
	//初期化
	player_->Initialize();
	//ハンドルの設定
	player_->SetLevelHandle(levelHandle_);
	//最初はコントロールは出来ない用にする
	player_->SetIsAbleToControll(false);

#pragma endregion

	//鍵のモデルの読み込み
	uint32_t keyModelHandle = modelManager_->Load("Resources/External/Model/key", "Key.obj");
	//生成
	keyManager_ = std::make_unique<KeyManager>();
	//レベルデータハンドルの設定
	keyManager_->SetLevelDataHandle(levelHandle_);
	//プレイヤーの設定
	keyManager_->SetPlayer(player_.get());

	//レベルデータから鍵の情報を取り出す
	std::vector<Vector3> keyPositions = levelDataManager_->GetObjectPositions(levelHandle_,"Key");
	keyManager_->Initialize(keyModelHandle, keyPositions);

#pragma region 敵
	//敵モデルの読み込み
	//通常
	uint32_t enemyModelHandle = modelManager_->Load("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "Ghost.gltf");
	//強敵
	uint32_t strongEnemyModelHandle = modelManager_->Load("Resources/External/Model/01_HalloweenItems00/01_HalloweenItems00/EditedGLTF", "StrongGhost.gltf");

	//敵管理システム
	enemyManager_ = std::make_unique<EnemyManager>();
	//プレイヤーの設定
	enemyManager_->SetPlayer(player_.get());
	//レベルデータ管理クラスの設定
	enemyManager_->SetLevelDataManager(levelDataManager_, levelHandle_);
	//初期化
	std::string initialPositionCSV = "Resources/CSV/EnemyInitialPosition.csv";
	//レベルデータから敵の情報を取り出す
	std::vector<Vector3> normalEnemyPositions = levelDataManager_->GetObjectPositions(levelHandle_, "NormalEnemy");
	std::vector<Vector3> strongEnemyPositions = levelDataManager_->GetObjectPositions(levelHandle_, "StrongEnemy");
	enemyManager_->Initialize(enemyModelHandle, strongEnemyModelHandle, normalEnemyPositions, strongEnemyPositions);
#pragma endregion

#pragma region カメラ

	//カメラの初期化
	camera_.Initialize();
	//それぞれに値を入れていく
	//回転
	//+で左回り
	camera_.rotate.y = std::numbers::pi_v<float_t> / 2.0f;
	
	
	

#pragma endregion

#pragma region 説明

	//説明画像の読み込み
	uint32_t explanationTextureHandle[EXPLANATION_QUANTITY_] = {};
	explanationTextureHandle[0] = textureManager_->Load("Resources/Sprite/Explanation/Explanation1.png");
	explanationTextureHandle[1] = textureManager_->Load("Resources/Sprite/Explanation/Explanation2.png");

	//生成
	for (uint32_t i = 0u; i < explanation_.size(); ++i) {
		explanation_[i].reset(Elysia::Sprite::Create(explanationTextureHandle[i], INITIAL_SPRITE_POSITION));
	}

	//スペースで次への画像読み込み
	uint32_t spaceToNextTextureHandle[EXPLANATION_QUANTITY_] = {};
	spaceToNextTextureHandle[0] = textureManager_->Load("Resources/Sprite/Explanation/ExplanationNext1.png");
	spaceToNextTextureHandle[1] = textureManager_->Load("Resources/Sprite/Explanation/ExplanationNext2.png");
	//生成
	for (uint32_t i = 0; i < spaceToNext_.size(); ++i) {
		spaceToNext_[i].reset(Elysia::Sprite::Create(spaceToNextTextureHandle[i], INITIAL_SPRITE_POSITION));
	}

	//最初は0番目
	howToPlayTextureNumber_ = 0u;

	//常時表示
	//操作
	uint32_t operationTextureHandle = textureManager_->Load("Resources/Sprite/Operation/Operation.png");
	//生成
	operation_.reset(Elysia::Sprite::Create(operationTextureHandle, { .x = 20.0f,.y = 0.0f }));


#pragma endregion

#pragma region UI
	

	//ゴールに向かえのテキスト
	uint32_t toEscapeTextureHandle = textureManager_->Load("Resources/Sprite/Escape/ToGoal.png");
	toEscape_.reset(Elysia::Sprite::Create(toEscapeTextureHandle, INITIAL_SPRITE_POSITION));

	//宝箱
	uint32_t openTreasureBoxSpriteHandle = textureManager_->Load("Resources/Sprite/TreasureBox/OpenTreasureBox.png");
	openTreasureBoxSprite_.reset(Elysia::Sprite::Create(openTreasureBoxSpriteHandle, INITIAL_SPRITE_POSITION));


#pragma endregion

	//コリジョン管理クラスの生成
	collisionManager_ = std::make_unique<Elysia::CollisionManager>();
	//角度の初期化
	//プレイヤーの向いている向きと合わせていくよ
	theta_ = std::numbers::pi_v<float_t> / 2.0f;

	//ポストエフェクトの初期化
	//ビネット生成
	vignette_ = std::make_unique<Elysia::Vignette>();
	//初期化
	vignette_->Initialize();
	//値の設定
	vignettePow_ = 0.0f;
	vignette_->SetPow(vignettePow_);

	//シーンのどこから始めるかを設定する
	isGamePlay_ = false;
	isExplain_ = false;


#ifdef _DEBUG 
	//デバッグするときはこっちで設定
	isWhiteFadeIn = false;
	isGamePlay_ = true;
#endif // _DEBUG


}


void GameScene::ObjectCollision() {

	//宝箱
	if (isOpenTreasureBox_==false) {
		const float_t RADIUS = 5.0f;
		Vector3 treasurePosition = levelDataManager_->GetInitialTranslate(levelHandle_, "TreasureBoxMain");

		if (player_->GetWorldPosition().x >= treasurePosition.x - RADIUS &&
			player_->GetWorldPosition().x <= treasurePosition.x + RADIUS &&
			player_->GetWorldPosition().z >= treasurePosition.z - RADIUS &&
			player_->GetWorldPosition().z <= treasurePosition.z + RADIUS) {

			//表示
			openTreasureBoxSprite_->SetInvisible(false);


			//Bボタンを押したとき
			if (input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
				//脱出
				isOpenTreasureBox_ = true;
			}

			//SPACEキーを押したとき
			if (input_->IsPushKey(DIK_SPACE) == true) {
				//脱出
				isOpenTreasureBox_ = true;
			}
		}
		else {
			//非表示
			openTreasureBoxSprite_->SetInvisible(true);
		}
	}
	//開いた動作
	else {
		//初期回転
		Vector3 initialRotate = levelDataManager_->GetInitialRotate(levelHandle_, "TreasureBoxLid");
		//回転
		Vector3 rotate = { .x = -std::numbers::pi_v<float_t> / 3.0f,.y = 0.0f,.z = 0.0f };
		//再設定
		levelDataManager_->SetRotate(levelHandle_, "TreasureBoxLid", VectorCalculation::Add(initialRotate, rotate));

	}

	//宝箱を開けたかどうかの設定
	keyManager_->SetIsOpenTreasureBox(isOpenTreasureBox_);
	//初期座標を取得
	Vector3 initialPosition = levelDataManager_->GetInitialTranslate(levelHandle_, "CloseFenceInCemetery");

	//墓場の鍵を取ったら柵が消える
	if (keyManager_ ->GetIsPickUpKeyInCemetery() == true) {
		translate_ = initialPosition;
	}
	//取っていないかつ墓場にいたら柵が下がり閉じ込められる
	else {
		if (player_->GetWorldPosition().z <= -26.0f &&
			player_->GetWorldPosition().z >= -50.0f) {
			translate_ = initialPosition;
			translate_.y = 0.0f;
			
		}
		else {
			translate_ = initialPosition;
		}
	}
	//座標の再設定
	levelDataManager_->SetTranslate(levelHandle_, "CloseFenceInCemetery", translate_);

#ifdef _DEBUG
	ImGui::Begin("墓場");
	ImGui::SliderFloat3("座標", &translate_.x, 0.0f, 100.0f);
	ImGui::End();
#endif // _DEBUG

}

void GameScene::EscapeCondition() {
	//ゲート
	if (gate_->isCollision(player_->GetWorldPosition())) {

		//3個取得したら脱出できる
		uint32_t playerKeyQuantity = player_->GetHavingKey();
		if (playerKeyQuantity >= keyManager_->GetMaxKeyQuantity()) {
			//「脱出せよ」が表示
			escapeText_->SetInvisible(false);

			//コントローラーのBボタンを押したら脱出のフラグがたつ
			//Bボタンを押したとき
			//SPACEキーを押したら脱出のフラグがたつ
			if (input_->IsPushButton(XINPUT_GAMEPAD_B) == true|| input_->IsPushKey(DIK_SPACE) == true) {
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

void GameScene::RegisterToCollisionManager() {

	//懐中電灯に対してのコライダーを登録
	collisionManager_->RegisterList(player_->GetFlashLightCollision());

	//エネミーをコリジョンマネージャーに追加
	//通常の敵のリストの取得
	std::vector<NormalEnemy*> enemyes = enemyManager_->GetNormalEnemies();
	for (const auto& enemy: enemyes) {
		//ダメージを受け入れる
		enemy->SetIsAcceptDamaged(isReleaseAttack_);
		if (isReleaseAttack_ == true) {
			//ライトの強さを反映
			enemy->SetLightStrength(player_->GetFlashLight()->GetChargeCondition());
			collisionManager_->RegisterList(enemy->GetEnemyFlashLightCollision());

		}
		
		//攻撃
		if (enemy->GetIsAttack() == true) {
			//敵の攻撃
			collisionManager_->RegisterList(enemy->GetEnemyAttackCollision());
			player_->SetIsAcceptDamegeFromNoemalEnemy(true);
		}
		else {
			player_->SetIsAcceptDamegeFromNoemalEnemy(false);
		}
		
	}


	//プレイヤーのコライダー
	std::vector<BasePlayerCollision*> playerColliders = player_->GetColliders();
	for (const auto& collider : playerColliders) {
		collisionManager_->RegisterList(collider);
	}


	

	std::vector<StrongEnemy*> strongEnemyes = enemyManager_->GetStrongEnemies();
	for (const StrongEnemy* strongEnemy : strongEnemyes) {
		bool isTouch = strongEnemy->GetStrongEnemyCollisionToPlayer()->GetIsTouchPlayer();
		collisionManager_->RegisterList(strongEnemy->GetStrongEnemyCollisionToPlayer());
		//接触
		if (isTouch == true) {
			isTouchStrongEnemy_ = true;
		}
	}

	std::vector<BaseObjectForLevelEditorCollider*> audioColliders = levelDataManager_->GetCollider(levelHandle_,"Audio");
	for (std::vector<BaseObjectForLevelEditorCollider*>::iterator it = audioColliders.begin(); it != audioColliders.end(); ++it) {
		collisionManager_->RegisterList(*it);

	}
}

void GameScene::VigntteProcess(){
	//HPが1でピンチの場合
	const uint32_t DANGEROUS_HP = 1u;
	//プレイヤーがダメージを受けた場合ビネット
	if (player_->GetIsDamaged() == true) {
		//時間の加算
		vignetteChangeTime_ += DELTA_TIME_;

		//線形補間で滑らかに変化
		vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, vignetteChangeTime_);
	}
	//ピンチ演出
	else if (player_->GetHP() == DANGEROUS_HP) {
		warningTime_ += DELTA_TIME_;
		vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, warningTime_);

		//最大時間
		const float_t MAX_WARNING_TIME = 1.0f;
		//最小時間
		const float_t MIN_WARNING_TIME = 1.0f;

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

}

void GameScene::DisplayImGui() {

	ImGui::Begin("ゲームシーン");
	ImGui::Checkbox("状態", &isReleaseAttack_);

	if (ImGui::TreeNode("ビネット")==true) {
		ImGui::InputFloat("POW", &vignettePow_);
		ImGui::InputFloat("変化の時間", &vignetteChangeTime_);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("フェード")==true) {
		ImGui::InputFloat("白", &whiteFadeTransparency_);
		ImGui::InputFloat("黒", &blackFadeTransparency_);
		ImGui::TreePop();
	}


	ImGui::End();

}

void GameScene::PlayerMove() {

	//何も押していない時つまり動いていないので通常はfalseと0にしておく
	
	//キーボードで動かしているかどうか
	bool isPlayerMoveKey = false;
	//動いているかどうか
	bool isPlayerMove = false;
	//向き
	Vector3 playerMoveDirection = { .x = 0.0f,.y = 0.0f,.z = 0.0f };

#pragma region キーボード
	//移動
	//Dキー(右)
	if (input_->IsPushKey(DIK_D) == true) {
		//動く方向
		playerMoveDirection = {
			.x = std::cosf(theta_ - std::numbers::pi_v<float_t> / 2.0f),
			.y = 0.0f,
			.z = std::sinf(theta_ - std::numbers::pi_v<float_t> / 2.0f),
		};

		//キーボード入力をしている
		isPlayerMoveKey = true;
		//動いている
		isPlayerMove = true;

	}
	//Aキー(左)
	if (input_->IsPushKey(DIK_A) == true) {
		//動く方向
		playerMoveDirection = {
			.x = std::cosf(theta_ + std::numbers::pi_v<float_t> / 2.0f),
			.y = 0.0f,
			.z = std::sinf(theta_ + std::numbers::pi_v<float_t> / 2.0f),
		};

		//キーボード入力をしている
		isPlayerMoveKey = true;
		//動いている
		isPlayerMove = true;
	}
	//Wキー(前)
	if (input_->IsPushKey(DIK_W) == true) {
		//動く方向
		playerMoveDirection = {
			.x = std::cosf(theta_),
			.y = 0.0f,
			.z = std::sinf(theta_),
		};

		//キーボード入力をしている
		isPlayerMoveKey = true;
		//動いている
		isPlayerMove = true;
	}
	//Sキー(後ろ)
	if (input_->IsPushKey(DIK_S) == true) {
		playerMoveDirection.x = std::cosf(theta_ + std::numbers::pi_v<float_t>);
		playerMoveDirection.z = std::sinf(theta_ + std::numbers::pi_v<float_t>);

		//キーボード入力をしている
		isPlayerMoveKey = true;
		//動いている
		isPlayerMove = true;
	}


#pragma endregion

#pragma region コントローラー

	//接続時
	//キーボード入力していない時かつ移動できる時に受け付ける
	if (isPlayerMoveKey == false) {

		//コントローラーの入力
		bool isInput = false;
		//左スティック
		Vector3 leftStickInput = {
			.x = (static_cast<float_t>(input_->GetCurrentState().Gamepad.sThumbLX) / SHRT_MAX),
			.y = 0.0f,
			.z = (static_cast<float_t>(input_->GetCurrentState().Gamepad.sThumbLY) / SHRT_MAX),
		};


		//デッドゾーンの設定
		const float_t DEAD_ZONE = 0.1f;
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
			isPlayerMove = true;

			//角度を求める
			float_t radian = std::atan2f(leftStickInput.z, leftStickInput.x);
			//値を0～2πに直してtheta_に揃える
			if (radian < 0.0f) {
				radian += 2.0f * std::numbers::pi_v<float_t>;
			}
			const float_t OFFSET = std::numbers::pi_v<float_t> / 2.0f;
			float_t resultTheta = theta_ + radian - OFFSET;


			//向きを代入
			playerMoveDirection.x = std::cosf(resultTheta);
			playerMoveDirection.z = std::sinf(resultTheta);
		}



	}

#pragma endregion

#pragma region

	//プレイヤーが動いている時
	if (isPlayerMove == true) {
		//ダッシュ
		//ダッシュしているかどうか
		bool isPlayerDash = false;
		if (isPlayerMoveKey == true) {
			if (input_->IsPushKey(DIK_RSHIFT) == true) {
				isPlayerDash = true;
			}
			else {
				isPlayerDash = false;
			}
		}
		//コントローラー接続時
		else {
			if (input_->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) == true) {
				isPlayerDash = true;
			}
			else {
				isPlayerDash = false;
			}
		}
		//ダッシュをしているかどうかの設定
		player_->SetIsDash(isPlayerDash);
	}
	//プレイヤーの動く方向を入れる
	player_->SetMoveDirection(playerMoveDirection);
	


	//チャージ
	bool isCharge = false;
	//エンターキーまたはYボタンでチャージ開始
	if(input_->IsPushKey(DIK_RETURN)==true|| input_->IsPushButton(XINPUT_GAMEPAD_Y) == true){
		isCharge = true;
	}
	else {
		isCharge = false;
	}

	//チャージ状態を設定
	player_->GetFlashLight()->SetIsCharge(isCharge);

	//エンターキーまたはYボタンを離した瞬間に攻撃する
	if (input_->IsReleaseKey(DIK_RETURN) == true|| input_->IsReleaseButton(XINPUT_GAMEPAD_Y) == true) {
		isReleaseAttack_ = true;
		//クールタイムにする
		player_->GetFlashLight()->SetIsCoolTime(true);
		//カメラの振動
		//攻撃できる時だけにする。その方が迫力が出るよね。
		if (player_->GetFlashLight()->GetChargeCondition() >= ChargeCondition::NormalChargeAttack) {
			player_->GetEyeCamera()->SetIsShake(true);
		}
	}
	else {
		isReleaseAttack_ = false;
	}
	
}

void GameScene::PlayerRotate() {

	//回転キーXY
	bool isRotateYKey = false;
	bool isRotateXKey = false;
	//回転の大きさ
	const float_t ROTATE_INTERVAL = 0.025f;

	//+が左回り
	//左を向く
	if (input_->IsPushKey(DIK_LEFT) == true) {
		theta_ += ROTATE_INTERVAL;
		isRotateYKey = true;
	}
	//右を向く
	if (input_->IsPushKey(DIK_RIGHT) == true) {
		theta_ -= ROTATE_INTERVAL;
		isRotateYKey = true;
	}
	//上を向く
	if (input_->IsPushKey(DIK_UP) == true) {
		phi_ -= ROTATE_INTERVAL;
		isRotateXKey = true;
	}
	//下を向く
	if (input_->IsPushKey(DIK_DOWN) == true) {
		phi_ += ROTATE_INTERVAL;
		isRotateXKey = true;
	}

	//2πより大きくなったら0にまた戻す
	if (theta_ > 2.0f * std::numbers::pi_v<float_t>) {
		theta_ = 0.0f;
	}
	//-2πより大きくなったら0にまた戻す
	if (theta_ < -2.0f * std::numbers::pi_v<float_t>) {
		theta_ = 0.0f;
	}

	

	

#pragma region コントローラーの回転
	

	//コントローラーがある場合
	const float_t MOVE_LIMITATION = 0.02f;

	//キーボード入力していない時
	if (isRotateYKey == false && isRotateXKey == false) {

		//入力
		float_t rotateMoveX = (static_cast<float_t>(input_->GetCurrentState().Gamepad.sThumbRY) / SHRT_MAX * ROTATE_INTERVAL);
		float_t rotateMoveY = (static_cast<float_t>(input_->GetCurrentState().Gamepad.sThumbRX) / SHRT_MAX * ROTATE_INTERVAL);

		//勝手に動くので制限を掛ける
		if (rotateMoveY < MOVE_LIMITATION && rotateMoveY > -MOVE_LIMITATION) {
			rotateMoveY = 0.0f;
		}
		if (rotateMoveX < MOVE_LIMITATION && rotateMoveX > -MOVE_LIMITATION) {
			rotateMoveX = 0.0f;
		}

		//補正後の値を代入する
		theta_ -= rotateMoveY;
		phi_ -= rotateMoveX;
	}



	


#pragma endregion

	//±π/6くらいに制限を掛けておきたい
	//それ以下以上だと首が大変なことになっているように見えるからね
	if (phi_ > std::numbers::pi_v<float_t> / 6.0f) {
		phi_ = std::numbers::pi_v<float_t> / 6.0f;
	}
	if (phi_ < -std::numbers::pi_v<float_t> / 6.0f) {
		phi_ = -std::numbers::pi_v<float_t> / 6.0f;
	}
}


void GameScene::Update(Elysia::GameManager* gameManager) {

	//フレーム初めに
	//コリジョンリストのクリア
	collisionManager_->ClearList();

	//フェードイン
	if (isWhiteFadeIn == true) {
		const float_t FADE_IN_INTERVAL = 0.01f;
		whiteFadeTransparency_ -= FADE_IN_INTERVAL;

		//完全に透明になったらゲームが始まる
		if (whiteFadeTransparency_ < PERFECT_TRANSPARENT_) {
			whiteFadeTransparency_ = PERFECT_TRANSPARENT_;
			isWhiteFadeIn = false;
			isExplain_ = true;
			//1枚目
			howToPlayTextureNumber_ = 1u;
		}
	}

	//ゲーム
	if (isWhiteFadeIn == false && isWhiteFadeOut_ == false) {
		whiteFadeTransparency_ = PERFECT_TRANSPARENT_;

		//説明
		if (isExplain_ == true) {
			//スペースまたはBボタンを押したとき
			if (input_->IsTriggerKey(DIK_SPACE) == true|| input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true) {
				++howToPlayTextureNumber_;
			}

			//読み終わったらゲームプレイへ
			if (howToPlayTextureNumber_ > MAX_EXPLANATION_NUMBER_) {
				isExplain_ = false;
				isGamePlay_ = true;
			}
		}
		//プレイ
		if (isGamePlay_ == true) {
			//コントロール可能にする
			player_->SetIsAbleToControll(true);
			//プレイヤーの移動
			PlayerMove();
			//回転
			PlayerRotate();

			//操作説明を追加
			isDisplayUI_ = true;
			//敵
			enemyManager_->Update();
			//敵を消す
			enemyManager_->DeleteEnemy();
		}

		//プレイヤーにそれぞれの角度を設定する
		player_->SetTheta(theta_);
		player_->SetPhi(phi_);

		//脱出の仕組み
		EscapeCondition();

		//オブジェクトの当たり判定
		ObjectCollision();

		//鍵
		keyManager_->Update();



		//体力が0になったら負け
		//または一発アウトの敵に接触した場合
		if (player_->GetIsAlive()==false || isTouchStrongEnemy_ == true) {
			//コントロールを失う
			player_->SetIsAbleToControll(false);
			//敵の音を止める
			enemyManager_->StopAudio();
			//鍵の音を止める
			keyManager_->StopAudio();

			//敵の動きが止まりブラックアウト
			isBlackFadeOut_ = true;
			blackFadeTransparency_ += FADE_OUT_INTERVAL_;
			blackFade_->SetTransparency(blackFadeTransparency_);


			//負けシーンへ
			if (blackFadeTransparency_ > CHANGE_TO_LOSE_SCENE_VALUE_) {
				gameManager->ChangeScene("Lose");
				return;
			}
		}
	}

	//ホワイトアウト
	if (isWhiteFadeOut_ == true) {

		//回転の値を加算
		const float_t ROTATE_VALUE = 0.01f;
		rightGateRotateTheta_ += ROTATE_VALUE;
		leftGateRotateTheta_ -= ROTATE_VALUE;

		std::string right = "GateDoorRight";
		std::string left = "GateDoorLeft";
		//門の回転
		levelDataManager_->SetRotate(levelHandle_, right, { .x = 0.0f,.y = rightGateRotateTheta_,.z = 0.0f });
		levelDataManager_->SetRotate(levelHandle_, left, { .x = 0.0f,.y = leftGateRotateTheta_,.z = 0.0f });

		//音を止める
		enemyManager_->StopAudio();
		//非表示
		escapeText_->SetInvisible(true);
		//振動しないようにする
		player_->SetIsAbleToControll(false);
		//加算
		whiteFadeTransparency_ += FADE_OUT_INTERVAL_;

		//最大の透明度
		const float_t MAX_TRANSPARENCY = 2.0f;
		if (whiteFadeTransparency_ > MAX_TRANSPARENCY) {
			gameManager->ChangeScene("Win");
			return;
		}
	}
	//フェードの透明度の設定
	whiteFade_->SetTransparency(whiteFadeTransparency_);

	//レベルエディタで使うリスナーの設定
	Listener listener = {
		.position = player_->GetWorldPosition(),
		.move = player_->GetDirection(),
	};
	levelDataManager_->SetListener(levelHandle_, listener);
	//レベルエディタの更新
	levelDataManager_->Update(levelHandle_);

	//プレイヤーの更新
	player_->Update();
	//門
	gate_->Update();

	//カメラの更新
	//レールカメラから2つの行列を取得
	camera_.viewMatrix = player_->GetEyeCamera()->GetCamera().viewMatrix;
	//転送
	camera_.Transfer();
	

	//ビネットの処理
	VigntteProcess();
	//コリジョン管理クラスに登録
	RegisterToCollisionManager();

	//当たり判定チェック
	collisionManager_->CheckAllCollision();


#ifdef _DEBUG 

	//再読み込み
	if (input_->IsTriggerKey(DIK_R) == true) {
		levelDataManager_->Reload(levelHandle_);
	}
	//ImGuiの表示
	DisplayImGui();

#endif // _DEBUG
}

void GameScene::PreDrawPostEffect() {
	//ビネット描画処理前
	vignette_->PreDraw();
}

void GameScene::DrawObject3D() {

	//懐中電灯を取得
	SpotLight spotLight = player_->GetFlashLight()->GetSpotLight();

	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_, spotLight);
	//敵
	enemyManager_->Draw(camera_, spotLight);
	//プレイヤー
	player_->DrawObject3D(camera_, spotLight);
	//鍵
	keyManager_->DrawObject3D(camera_, spotLight);
}

void GameScene::DrawPostEffect() {
	//ビネット描画
	vignette_->Draw();
}

void GameScene::DrawSprite() {

	
	//説明
	for (size_t i = 0u; i < explanation_.size(); ++i) {
		if (howToPlayTextureNumber_ == i + 1) {
			explanation_[i]->Draw();
			spaceToNext_[i]->Draw();
		}
	}

	//UIを表示するかどうか
	if (isDisplayUI_ == true) {
		//プレイヤー
		player_->DrawSprite();

		//操作説明
		operation_->Draw();
		//鍵
		keyManager_->DrawSprite();
		//脱出
		escapeText_->Draw();
		
		if (player_->GetHavingKey() == keyManager_->GetMaxKeyQuantity()) {
			toEscape_->Draw();
		}
	}

	//フェード
	if (isWhiteFadeIn == true || isWhiteFadeOut_ == true) {
		whiteFade_->Draw();
	}
	//黒フェード
	if (isBlackFadeIn == true || isBlackFadeOut_ == true) {
		blackFade_->Draw();
	}


}
