#include "Player.h"
#include <Input.h>
#include <VectorCalculation.h>

#include "Material.h"
#include "SpotLight.h"
#include <numbers>

#include "Stage/ObjectManager/ObjectManager.h"
#include "GameScene/GameScene.h"
#include <SingleCalculation.h>
#include <Collider/CollisionConfig.h>
#include <ModelManager.h>

void Player::Initialize(){


	//モデルの生成 //"C:\Lesson\CG\CGGrade3\Ellysia_3.0\Resources\Sample\Cube\cube.obj"
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","cube.obj");
	model_.reset(Model::Create(modelHandle));

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



	//通常
	colliderToNormalEnemy_ = std::make_unique<PlayerCollisionToNormalEnemyAttack>();
	colliderToNormalEnemy_->Initialize();


	//強敵
	collisionToStrongEnemy_ = std::make_unique<PlayerCollisionToStrongEnemy>();
	collisionToStrongEnemy_->Initialize();


	flashLight_ = std::make_unique<FlashLight>();
	flashLight_->Initialize();


	//ダメージを受けたかどうか
	isDameged_ = false;
	

	//マテリアル
	material_.Initialize();
	material_.lightingKinds_ = Spot;
}



void Player::Damaged() {
	//通常の敵に当たった場合
	if (colliderToNormalEnemy_->GetIsTouch() == true) {
		//ダメージを受ける
		if (!isDameged_) {
			acceptDamage_ = true;
			isDameged_ = true;
		}

	}
	else {
		acceptDamage_ = false;
	}


	//
	if (acceptDamage_ == true) {


		//体力を1減らす
		if (isDameged_ == true) {
			--hp_;
			isDameged_ = false; // HPを減らした後、振動処理に移行
		}

		//線形補間で振動処理をする
		vibeTime_ += DELTA_TIME;
		vibeStrength_ = SingleCalculation::Lerp(1.0f, 0.0f, vibeTime_);
		Input::GetInstance()->SetVibration(vibeStrength_, vibeStrength_);

		//振動を止める
		if (vibeStrength_ <= 0.0f) {
			Input::GetInstance()->StopVibration();
			vibeTime_ = 0.0f;
			isDameged_ = false;
		}
	}
}
void Player::Update(){
	


	//動けるときだけ加算
	if (isControll_ == true && moveCondition_==PlayerMoveCondition::OnPlayerMove) {
	

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
	

	//ステージの外には行けないようにする
	//左
	if (worldTransform_.translate.x < stageRect_.leftBack.x + SIDE_SIZE) {
		worldTransform_.translate.x = stageRect_.leftBack.x + SIDE_SIZE;
	}
	//右
	if (worldTransform_.translate.x > stageRect_.rightBack.x - SIDE_SIZE) {
		worldTransform_.translate.x = stageRect_.rightBack.x - SIDE_SIZE;
	}
	//奥
	if (worldTransform_.translate.z > stageRect_.leftBack.z - SIDE_SIZE) {
		worldTransform_.translate.z = stageRect_.leftBack.z - SIDE_SIZE;
	}
	//手前
	if (worldTransform_.translate.z < stageRect_.leftFront.z + SIDE_SIZE) {
		worldTransform_.translate.z = stageRect_.leftFront.z + SIDE_SIZE;
	}

	//ワールドトランスフォームの更新
	worldTransform_.Update();
	//AABBの計算
	aabb_.min.x = worldTransform_.GetWorldPosition().x - SIDE_SIZE;
	aabb_.min.y = worldTransform_.GetWorldPosition().y - SIDE_SIZE;
	aabb_.min.z = worldTransform_.GetWorldPosition().z - SIDE_SIZE;

	aabb_.max.x = worldTransform_.GetWorldPosition().x + SIDE_SIZE;
	aabb_.max.y = worldTransform_.GetWorldPosition().y + SIDE_SIZE;
	aabb_.max.z = worldTransform_.GetWorldPosition().z + SIDE_SIZE;

	//通常の敵用の当たり判定の更新
	colliderToNormalEnemy_->SetPlayerPosition(worldTransform_.GetWorldPosition());
	colliderToNormalEnemy_->Update();

	//一発アウト用の当たり判定
	collisionToStrongEnemy_->SetPlayerPosition(worldTransform_.GetWorldPosition());
	collisionToStrongEnemy_->Update();

	//懐中電灯の更新
	//角度はゲームシーンで取得する
	flashLight_->SetPlayerPosition(worldTransform_.GetWorldPosition());
	flashLight_->Update();


	//マテリアルの更新
	material_.Update();

	//攻撃を受ける
	Damaged();
	

	
#ifdef _DEBUG
	//ImGuiにInputUintが無いの不便・・
	int32_t keyQuantity = static_cast<int32_t>(haveKeyQuantity_);
	int32_t condition = static_cast<int32_t>(moveCondition_);


	ImGui::Begin("プレイヤー");
	if (ImGui::TreeNode("状態")) {
		ImGui::InputInt("鍵の数", &keyQuantity);
		ImGui::InputInt("体力", &hp_);
		ImGui::Checkbox("acceptDamage_", &acceptDamage_);
		ImGui::Checkbox("isDamage_", &isDamage_);
		ImGui::Checkbox("振動", &isDameged_);

		ImGui::TreePop();
	}
	ImGui::InputInt("downTime", &downTime_);
	ImGui::InputFloat3("Transrate", &worldTransform_.translate.x);
	ImGui::InputFloat3("MoveDirection", &moveDirection_.x);
	ImGui::InputInt("moveCondition_", &condition);
	ImGui::End();
#endif

}

void Player::Draw(const Camera& camera, const SpotLight& spotLight){

#ifdef _DEBUG
	//本体の描画
	//1人称視点だからいらないかもね
	model_->Draw(worldTransform_, camera,material_,spotLight);

	//通常
	colliderToNormalEnemy_->Draw(camera, material_, spotLight);
	//強敵	
	collisionToStrongEnemy_->Draw(camera, material_, spotLight);
	//懐中電灯
	flashLight_->Draw(camera);
#endif // _DEBUG


}


Player::~Player() {
	Input::GetInstance()->StopVibration();
}


