#include "Player.h"

#include <numbers>

#include "Input.h"
#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "ModelManager.h"
#include "SpotLight.h"
#include "LevelDataManager.h"
#include "PushBackCalculation.h"

Player::Player(){

	//インスタンスの取得
	//入力クラス
	input_ = Elysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Elysia::ModelManager::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Elysia::LevelDataManager::GetInstance();
}

void Player::Initialize(){

	//モデルの生成 	
	uint32_t modelHandle = modelManager_->LoadModelFile("Resources/Model/Sample/Cube","cube.obj");
	model_.reset(Elysia::Model::Create(modelHandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const Vector3 INITIAL_POSITION = { .x=0.0f,.y=0.0f,.z=-15.0f };
	worldTransform_.translate = INITIAL_POSITION;

	//通常の敵
	std::unique_ptr<PlayerCollisionToNormalEnemyAttack> colliderToNormalEnemy = std::make_unique<PlayerCollisionToNormalEnemyAttack>();
	colliderToNormalEnemy->Initialize();
	colliders_.push_back(std::move(colliderToNormalEnemy));
	//強敵
	std::unique_ptr<PlayerCollisionToStrongEnemy> collisionToStrongEnemy = std::make_unique<PlayerCollisionToStrongEnemy>();
	collisionToStrongEnemy->Initialize();
	colliders_.push_back(std::move(collisionToStrongEnemy));
	//オーディオオブジェクト
	std::unique_ptr<PlayerCollisionToAudioObject> collosionToAudioObject= std::make_unique<PlayerCollisionToAudioObject>();
	collosionToAudioObject->Initialize();
	colliders_.push_back(std::move(collosionToAudioObject));

	//懐中電灯
	flashLight_ = std::make_unique<FlashLight>();
	flashLight_->Initialize();

	
	//マテリアル
	material_.Initialize();
	material_.lightingKinds = SpotLighting;
	material_.color = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 0.5f };
}

void Player::Update(){
	
	//移動処理
	Move();

	//攻撃を受ける
	Damaged();

	//ワールドトランスフォームの更新
	worldTransform_.translate = playerCenterPosition_;
	//Yを固定させる
	const float HEIGHT = 0.0f;
	worldTransform_.translate.y = HEIGHT;
	worldTransform_.Update();

	//ワールド座標
	Vector3 worldPosition = worldTransform_.GetWorldPosition();

	//AABBの計算
	aabb_.min = VectorCalculation::Subtract(worldPosition, { SIDE_SIZE ,SIDE_SIZE ,SIDE_SIZE });
	aabb_.max = VectorCalculation::Add(worldPosition, { SIDE_SIZE ,SIDE_SIZE ,SIDE_SIZE });
	//コリジョンの更新
	for (std::unique_ptr<BasePlayerCollision> &collision : colliders_) {
		//プレイヤーの座標を設定
		collision->SetPlayerPosition(worldPosition);
		//更新
		collision->Update();
	}
	//懐中電灯の更新
	//角度はゲームシーンで取得する
	flashLight_->SetPlayerPosition(worldPosition);
	flashLight_->Update();

	//マテリアルの更新
	material_.Update();

	
	#ifdef _DEBUG

	//ImGui表示
	DisplayImGui();
	#endif
}

void Player::Draw(const Camera& camera, const SpotLight& spotLight){

#ifdef _DEBUG
	//本体の描画
	//1人称視点だからいらないね
	model_->Draw(worldTransform_, camera,material_,spotLight);

	//懐中電灯
	flashLight_->Draw(camera);
#endif // _DEBUG


}

Player::~Player() {
	//振動を止める
	input_->StopVibration();
}

void Player::Damaged() {

	//通常の敵に当たった場合
	if (isAcceptDamegeFromNoemalEnemy_ == true && isDameged_ == false) {
		//体力を減らす
		--hp_;
		//ダメージを受ける	
		isDameged_ = true;
	}


	if (isDameged_ == true) {

		//一時的にコントロールを失う
		isControll_ = false;
		//線形補間で振動処理をする
		vibeTime_ += DELTA_TIME;
		//最大の振動の強さ
		const float MAX_VIBE_ = 1.0f;
		//最小の振動の強さ
		const float MIN_VIBE_ = 0.0f;

		//線形補間を使い振動を減衰させる
		//振動の強さ
		float vibeStrength_ =  SingleCalculation::Lerp(MAX_VIBE_, MIN_VIBE_, vibeTime_);
		//振動の設定
		input_->SetVibration(vibeStrength_, vibeStrength_);

		//振動を止める
		if (vibeStrength_ <= MIN_VIBE_) {
			//振動が止まる
			input_->StopVibration();

			//戻る時間
			const float RESTART_TIME = 0.0f;
			//時間を戻す
			vibeTime_ = RESTART_TIME;
			//ダメージを受けていないようにする
			isDameged_ = false;

			//コントロールを戻す
			isControll_ = true;
		}

#ifdef _DEBUG
		ImGui::Begin("振動");
		ImGui::InputFloat("T", &vibeStrength_);
		ImGui::End();
#endif // _DEBUG
	}
}

void Player::Move() {
	//動けるときだけ加算
	if (isControll_ == true ) {

		//歩くスピード
		const float NORMAL_MOVE_SPEED = 0.1f;
		//走るスピード
		const float DASH_MOVE_SPEED = 0.2f;

		float moveSpeed = 0.0f;
		//走っている時
		if (isDash_ == true) {
			moveSpeed = DASH_MOVE_SPEED;
		}
		//通常の動きの時
		else {
			moveSpeed = NORMAL_MOVE_SPEED;
		}
		//加算
		playerCenterPosition_ = VectorCalculation::Add(playerCenterPosition_, VectorCalculation::Multiply(moveDirection_, moveSpeed));
		
		//AABB
		std::vector<AABB> aabbs = levelDataManager_->GetObjectAABBs(levelHandle_,"Stage");
		//コライダーを持っているかどうか
		std::vector<bool> colliders = levelDataManager_->GetIsHavingColliders(levelHandle_, "Stage");
		//衝突判定
		for (size_t i = 0; i < aabbs.size(); ++i) {

			//コライダーを持っているときだけ
			if (colliders[i] == true) {
				//押し戻し処理
				PushBackCalculation::FixPosition(playerCenterPosition_, aabb_, aabbs[i]);
			}
		}

	}
}

void Player::DisplayImGui() {

	//それぞれintに変換
	int32_t keyQuantity = static_cast<int32_t>(haveKeyQuantity_);

	ImGui::Begin("プレイヤー");
	if (ImGui::TreeNode("状態")==true) {
		ImGui::InputInt("鍵の数", &keyQuantity);
		ImGui::InputInt("体力", &hp_);
		ImGui::Checkbox("isDamage_", &isDamage_);
		ImGui::Checkbox("振動", &isDameged_);
		ImGui::TreePop();
	}

	ImGui::Checkbox("敵からの攻撃を受け入れるか", &isAcceptDamegeFromNoemalEnemy_);
	ImGui::InputInt("downTime", &downTime_);
	ImGui::InputFloat3("Transrate", &worldTransform_.translate.x);
	ImGui::InputFloat3("MoveDirection", &moveDirection_.x);
	ImGui::End();

}

