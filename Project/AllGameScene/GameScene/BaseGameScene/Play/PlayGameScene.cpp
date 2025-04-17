#include "PlayGameScene.h"

#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "TextureManager.h"
#include "LevelDataManager.h"
#include "VectorCalculation.h"
#include "GameScene/GameScene.h"



PlayGameScene::PlayGameScene(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}


void PlayGameScene::Initialize(){
	//空の場合停止させる
	assert(player_);
	assert(enemyManager_);
	assert(keyManager_);

	//コリジョン管理クラスの生成
	collisionManager_ = std::make_unique<Elysia::CollisionManager>();
}

void PlayGameScene::Update(GameScene* gameScene){
	//フレーム初めに
	//コリジョンリストのクリア
	collisionManager_->ClearList();

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
	if (player_->GetIsAlive() == false || isTouchStrongEnemy_ == true) {
		//コントロールを失う
		player_->SetIsAbleToControll(false);
		//敵の音を止める
		enemyManager_->StopAudio();
		//鍵の音を止める
		keyManager_->StopAudio();

		//敵の動きが止まりブラックアウト

		//負けシーンへ
		if (false) {
			//gameScene->ChangeDetailScene();
			return;
		}
	}


	//ビネットの処理
	VigntteProcess();
	//コリジョン管理クラスに登録
	RegisterToCollisionManager();

	//当たり判定チェック
	collisionManager_->CheckAllCollision();

#ifdef _DEBUG
	//ImGui表示用
	DisplayImGui();
#endif // _DEBUG
}


void PlayGameScene::DrawSprite(){


}

void PlayGameScene::RegisterToCollisionManager(){

	//懐中電灯に対してのコライダーを登録
	collisionManager_->RegisterList(player_->GetFlashLightCollision());

	//エネミーをコリジョンマネージャーに追加
	//通常の敵のリストの取得
	std::vector<NormalEnemy*> enemyes = enemyManager_->GetNormalEnemies();
	for (const auto& enemy : enemyes) {
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



	//強敵
	std::vector<StrongEnemy*> strongEnemyes = enemyManager_->GetStrongEnemies();
	for (const StrongEnemy* strongEnemy : strongEnemyes) {
		bool isTouch = strongEnemy->GetStrongEnemyCollisionToPlayer()->GetIsTouchPlayer();
		collisionManager_->RegisterList(strongEnemy->GetStrongEnemyCollisionToPlayer());
		//接触
		if (isTouch == true) {
			isTouchStrongEnemy_ = true;
		}
	}

	//レベルエディタのオーディオ用
	std::vector<BaseObjectForLevelEditorCollider*> audioColliders = levelDataManager_->GetCollider(levelDataHandle_, "Audio");
	for (std::vector<BaseObjectForLevelEditorCollider*>::iterator it = audioColliders.begin(); it != audioColliders.end(); ++it) {
		collisionManager_->RegisterList(*it);

	}
}

void PlayGameScene::PlayerMove(){

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
	//エンターキーまたはXボタンでチャージ開始
	if (input_->IsPushKey(DIK_RETURN) == true || input_->IsPushButton(XINPUT_GAMEPAD_X) == true) {
		isCharge = true;
	}
	else {
		isCharge = false;
	}

	//チャージ状態を設定
	player_->GetFlashLight()->SetIsCharge(isCharge);

	//エンターキーまたはYボタンを離した瞬間に攻撃する
	if (input_->IsReleaseKey(DIK_RETURN) == true || input_->IsReleaseButton(XINPUT_GAMEPAD_X) == true) {
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

void PlayGameScene::PlayerRotate(){

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

void PlayGameScene::EscapeCondition(){

}

void PlayGameScene::ObjectCollision(){

	//宝箱
	if (isOpenTreasureBox_ == false) {
		const float_t RADIUS = 5.0f;
		Vector3 treasurePosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, "TreasureBoxMain");

		if (player_->GetWorldPosition().x >= treasurePosition.x - RADIUS &&
			player_->GetWorldPosition().x <= treasurePosition.x + RADIUS &&
			player_->GetWorldPosition().z >= treasurePosition.z - RADIUS &&
			player_->GetWorldPosition().z <= treasurePosition.z + RADIUS) {

			//表示
			//openTreasureBoxSprite_->SetInvisible(false);


			//Bボタンまたはスペースキーを押したとき
			if (input_->IsTriggerButton(XINPUT_GAMEPAD_B) == true|| input_->IsTriggerKey(DIK_SPACE) == true) {
				//脱出
				isOpenTreasureBox_ = true;
			}

		}
		else {
			//非表示
			//openTreasureBoxSprite_->SetInvisible(true);
		}
	}
	//開いた動作
	else {
		//初期回転
		Vector3 initialRotate = levelDataManager_->GetInitialRotate(levelDataHandle_, "TreasureBoxLid");
		//回転
		Vector3 rotate = { .x = -std::numbers::pi_v<float_t> / 3.0f,.y = 0.0f,.z = 0.0f };
		//再設定
		levelDataManager_->SetRotate(levelDataHandle_, "TreasureBoxLid", VectorCalculation::Add(initialRotate, rotate));

	}

	//宝箱を開けたかどうかの設定
	keyManager_->SetIsOpenTreasureBox(isOpenTreasureBox_);
	//初期座標を取得
	Vector3 initialPosition = levelDataManager_->GetInitialTranslate(levelDataHandle_, "CloseFenceInCemetery");

	//墓場の鍵を取ったら柵が消える
	if (keyManager_->GetIsPickUpKeyInCemetery() == true) {
		fenceTranslate_ = initialPosition;
	}
	//取っていないかつ墓場にいたら柵が下がり閉じ込められる
	else {
		if (player_->GetWorldPosition().z <= -26.0f &&
			player_->GetWorldPosition().z >= -50.0f) {
			fenceTranslate_ = initialPosition;
			fenceTranslate_.y = 0.0f;

		}
		else {
			fenceTranslate_ = initialPosition;
		}
	}
	//座標の再設定
	levelDataManager_->SetTranslate(levelDataHandle_, "CloseFenceInCemetery", fenceTranslate_);


}

void PlayGameScene::VigntteProcess(){
	////HPが1でピンチの場合
	//const uint32_t DANGEROUS_HP = 1u;
	////プレイヤーがダメージを受けた場合ビネット
	//if (player_->GetIsDamaged() == true) {
	//	//時間の加算
	//	vignetteChangeTime_ += DELTA_ANIMATION_TIME_;
	//
	//	//線形補間で滑らかに変化
	//	vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, vignetteChangeTime_);
	//}
	////ピンチ演出
	//else if (player_->GetHP() == DANGEROUS_HP) {
	//	warningTime_ += DELTA_ANIMATION_TIME_;
	//	vignettePow_ = SingleCalculation::Lerp(MAX_VIGNETTE_POW_, 0.0f, warningTime_);
	//
	//	//最大時間
	//	const float_t MAX_WARNING_TIME = 1.0f;
	//	//最小時間
	//	const float_t MIN_WARNING_TIME = 1.0f;
	//
	//	if (warningTime_ > MAX_WARNING_TIME) {
	//		warningTime_ = MIN_WARNING_TIME;
	//	}
	//}
	////通常時の場合
	//else {
	//	vignettePow_ = 0.0f;
	//	vignetteChangeTime_ = 0.0f;
	//}
	//vignette_->SetPow(vignettePow_);
}

void PlayGameScene::DisplayImGui(){

	ImGui::Begin("プレイ(ゲーム)");
	ImGui::SliderFloat3("座標", &fenceTranslate_.x, 0.0f, 100.0f);

	ImGui::End();

}
