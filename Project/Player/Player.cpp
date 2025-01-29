#include "Player.h"
#include <numbers>


#include "Input.h"
#include "VectorCalculation.h"
#include "Material.h"
#include "SpotLight.h"
#include "GameScene/GameScene.h"
#include "SingleCalculation.h"
#include "CollisionConfig.h"
#include "ModelManager.h"

Player::Player(){
	//入力クラスの取得
	input_ = Ellysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Ellysia::ModelManager::GetInstance();
}

void Player::Initialize(){

	//モデルの生成 	
	uint32_t modelHandle = modelManager_->LoadModelFile("Resources/Model/Sample/Cube","cube.obj");
	model_.reset(Ellysia::Model::Create(modelHandle));

	//初期はコントロールできない
	isControll_ = false;

	//持っている鍵の数
	haveKeyQuantity_ = 0u;
	
	//体力
	hp_ = 3u;

	//ダメージについて
	isDamage_ = false;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const Vector3 INITIAL_POSITION = { .x=0.0f,.y=0.0f,.z=-15.0f };
	worldTransform_.translate = INITIAL_POSITION;

	//PlayerCllsion基底クラスを作ろう
	//通常の敵
	std::unique_ptr<PlayerCollisionToNormalEnemyAttack> colliderToNormalEnemy = std::make_unique<PlayerCollisionToNormalEnemyAttack>();
	colliderToNormalEnemy->Initialize();
	//そのまま入れることは出来ないので所有権を移すstd::moveを使うよ
	colliders_.push_back(std::move(colliderToNormalEnemy));


	//通常
	//colliderToNormalEnemy_ = std::make_unique<PlayerCollisionToNormalEnemyAttack>();
	//colliderToNormalEnemy_->Initialize();

	//強敵
	std::unique_ptr<PlayerCollisionToStrongEnemy> collisionToStrongEnemy = std::make_unique<PlayerCollisionToStrongEnemy>();
	collisionToStrongEnemy->Initialize();
	colliders_.push_back(std::move(collisionToStrongEnemy));
	//collisionToStrongEnemy_ = std::make_unique<PlayerCollisionToStrongEnemy>();
	//collisionToStrongEnemy_->Initialize();
	//

	//オーディオオブジェクト
	std::unique_ptr<PlayerCollisionToAudioObject> collosionToAudioObject= std::make_unique<PlayerCollisionToAudioObject>();
	collosionToAudioObject->Initialize();
	colliders_.push_back(std::move(collosionToAudioObject));
	//collosionToAudioObject_ = std::make_unique<PlayerCollisionToAudioObject>();
	//collosionToAudioObject_->Initialize();


	//懐中電灯
	flashLight_ = std::make_unique<FlashLight>();
	flashLight_->Initialize();


	//ダメージを受けたかどうか
	isDameged_ = false;
	
	//マテリアル
	material_.Initialize();
	material_.lightingKinds_ = SpotLighting;
}

void Player::Update(){
	
	//移動処理
	Move();

	//攻撃を受ける
	Damaged();

	//ワールドトランスフォームの更新
	worldTransform_.Update();

	//ワールド座標
	Vector3 worldPosition = worldTransform_.GetWorldPosition();

	//AABBの計算
	aabb_.min.x = worldPosition.x - SIDE_SIZE;
	aabb_.min.y = worldPosition.y - SIDE_SIZE;
	aabb_.min.z = worldPosition.z - SIDE_SIZE;

	aabb_.max.x = worldPosition.x + SIDE_SIZE;
	aabb_.max.y = worldPosition.y + SIDE_SIZE;
	aabb_.max.z = worldPosition.z + SIDE_SIZE;

	//PMで綺麗にまとめられるかも
	//通常の敵用の当たり判定の更新
	for (std::unique_ptr<BasePlayerCollision> &collision : colliders_) {
		//プレイヤーの座標を設定
		collision->SetPlayerPosition(worldPosition);
		//更新
		collision->Update();
	}

	//colliderToNormalEnemy_->SetPlayerPosition(worldPosition);
	//colliderToNormalEnemy_->Update();
	//
	////一発アウト用の当たり判定
	//collisionToStrongEnemy_->SetPlayerPosition(worldPosition);
	//collisionToStrongEnemy_->Update();
	//
	////オーディオオブジェクトに対しての当たり判定
	//collosionToAudioObject_->SetPlayerPosition(worldPosition);
	//collosionToAudioObject_->Update();

	//懐中電灯の更新
	//角度はゲームシーンで取得する
	flashLight_->SetPlayerPosition(worldPosition);
	flashLight_->Update();

	//マテリアルの更新
	material_.Update();

	//ImGui表示
#ifdef _DEBUG
	DisplayImGui();
#endif
}

void Player::Draw(const Camera& camera, const SpotLight& spotLight){

#ifdef _DEBUG
	//本体の描画
	//1人称視点だからいらないね
	model_->Draw(worldTransform_, camera,material_,spotLight);

	//通常
	//colliderToNormalEnemy_->Draw(camera, material_, spotLight);
	//強敵	
	//collisionToStrongEnemy_->Draw(camera, material_, spotLight);
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
	//if (colliderToNormalEnemy_->GetIsTouch() == true) {

		//ダメージを受ける
		if (isAcceptDamegeFromNoemalEnemy_ == true && isDameged_ == false) {
			//一時的にコントロールを失う
			isControll_ = false;
			//ダメージを受ける
			isDameged_ = true;
			//体力を減らす
			--hp_;
			//線形補間で振動処理をする
			vibeTime_ += DELTA_TIME;

			//最大の振動の強さ
			const float MAX_VIBE_ = 1.0f;
			//最小の振動の強さ
			const float MIN_VIBE_ = 0.0f;

			//線形補間を使い振動を減衰させる
			vibeStrength_ = SingleCalculation::Lerp(MAX_VIBE_, MIN_VIBE_, vibeTime_);
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
		}

	//}
	//else {
	//	//ダメージを受けない
	//	//当たっていないので
	//	isDameged_ = false;
	//
	//}
}

void Player::Move() {
	//動けるときだけ加算
	if (isControll_ == true && moveCondition_ == PlayerMoveCondition::OnPlayerMove) {

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
		worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, VectorCalculation::Multiply(moveDirection_, moveSpeed));

	}
}

void Player::DisplayImGui() {

	//それぞれintに変換
	int32_t keyQuantity = static_cast<int32_t>(haveKeyQuantity_);
	int32_t condition = static_cast<int32_t>(moveCondition_);

	ImGui::Begin("プレイヤー");
	if (ImGui::TreeNode("状態")) {
		ImGui::InputInt("鍵の数", &keyQuantity);
		ImGui::InputInt("体力", &hp_);
		ImGui::InputInt("ダメージ時間", &damagedTime_);

		ImGui::Checkbox("isDamage_", &isDamage_);
		ImGui::Checkbox("振動", &isDameged_);

		ImGui::TreePop();
	}

	ImGui::Checkbox("敵からの攻撃を受け入れるか", &isAcceptDamegeFromNoemalEnemy_);
	ImGui::InputInt("downTime", &downTime_);
	ImGui::InputFloat3("Transrate", &worldTransform_.translate.x);
	ImGui::InputFloat3("MoveDirection", &moveDirection_.x);
	ImGui::InputInt("moveCondition_", &condition);
	ImGui::End();

}

