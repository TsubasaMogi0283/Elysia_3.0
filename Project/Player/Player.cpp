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



	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/TD3Player","Player.obj");
	model_.reset(Model::Create(modelHandle));

	isControll_ = false;

	//持っている鍵の数
	haveKeyQuantity_ = 0u;
	
	//体力
	hp_ = 3u;

	//ダメージについて
	acceptDamage_ = true;
	isDamage_ = false;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const Vector3 INITIAL_POSITION = { .x=0.0f,.y=0.0f,.z=-15.0f };
	worldTransform_.translate_ = INITIAL_POSITION;

	#pragma region 当たり判定について

	//種類
	collisionType_ = CollisionType::SphereType;

	//半径
	radius_ = 1.0f;

	//AABBのmax部分に加算する縦横高さのサイズ
	upSideSize_ = {1.0f,1.0f,1.0f};

	//AABBのmin部分に加算する縦横高さのサイズ
	downSideSize_ = {1.0f,1.0f,1.0f};

	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY_ATTACK);


	//一発アウト用
	collisionToStrongEnemy_ = std::make_unique<PlayerCollisionToStrongEnemy>();
	collisionToStrongEnemy_->Initialize();

	#pragma endregion


	
	isDameged_ = false;
	
}

void Player::Update(){

	const float NORMAL_MOVE_SPEED = 0.1f;
	const float DASH_MOVE_SPEED = 0.2f;



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
		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, VectorCalculation::Multiply(moveDirection_, moveSpeed));

	}
	

	//ステージの外には行けないようにする
	//左
	if (worldTransform_.translate_.x < stageRect_.leftBack.x + radius_) {
		worldTransform_.translate_.x = stageRect_.leftBack.x + radius_;
	}
	//右
	if (worldTransform_.translate_.x > stageRect_.rightBack.x - radius_) {
		worldTransform_.translate_.x = stageRect_.rightBack.x - radius_;
	}
	//奥
	if (worldTransform_.translate_.z > stageRect_.leftBack.z - radius_) {
		worldTransform_.translate_.z = stageRect_.leftBack.z - radius_;
	}
	//手前
	if (worldTransform_.translate_.z < stageRect_.leftFront.z + radius_) {
		worldTransform_.translate_.z = stageRect_.leftFront.z + radius_;
	}

	//ワールドトランスフォームの更新
	worldTransform_.Update();


	aabb_.min.x = GetWorldPosition().x - downSideSize_.x;
	aabb_.min.y = GetWorldPosition().y - downSideSize_.y;
	aabb_.min.z = GetWorldPosition().z - downSideSize_.z;

	aabb_.max.x = GetWorldPosition().x + downSideSize_.x;
	aabb_.max.y = GetWorldPosition().y + downSideSize_.y;
	aabb_.max.z = GetWorldPosition().z + downSideSize_.z;

	//一発アウト用の当たり判定
	collisionToStrongEnemy_->SetPlayerPosition(GetWorldPosition());
	collisionToStrongEnemy_->Update();


	//攻撃された場合
	if (isDameged_ == true) {
		//線形補間で振動処理をする
		const float DELTA_TIME = 1.0f/60.0f;
		vibeTime_ += DELTA_TIME;
		vibeStrength_ = SingleCalculation::Lerp(1.0f, 0.0f, vibeTime_);
		Input::GetInstance()->SetVibration(vibeStrength_, vibeStrength_);

		//振動を止める
		if (vibeStrength_ < 0.0f) {
			Input::GetInstance()->StopVibration();
			vibeTime_ = 0.0f;
			isDameged_ = false;
		}
	}

	
#ifdef _DEBUG
	//ImGuiにInputUintが無いの不便・・
	int keyQuantity = haveKeyQuantity_;
	int condition = moveCondition_;


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
	ImGui::InputFloat3("Transrate", &worldTransform_.translate_.x);
	ImGui::InputFloat3("MoveDirection", &moveDirection_.x);
	ImGui::InputInt("moveCondition_", &condition);
	ImGui::End();
#endif

}

void Player::Draw(Camera& camera, Material& material, SpotLight& spotLight){
	
	model_->Draw(worldTransform_, camera,material,spotLight);

#ifdef _DEBUG
	collisionToStrongEnemy_->Draw(camera, spotLight);
#endif // _DEBUG


}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPosition = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};
	return worldPosition;
}

void Player::OnCollision(){
	//体力を1減らす
	--hp_;

	//振動させる
	isDameged_ = true;
}

void Player::OffCollision(){

}

Player::~Player() {
	Input::GetInstance()->StopVibration();
}