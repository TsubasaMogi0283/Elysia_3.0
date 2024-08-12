#include "Player.h"
#include <Input.h>
#include <VectorCalculation.h>

#include "Material.h"
#include "SpotLight.h"
#include <numbers>

#include "Stage/ObjectManager/ObjectManager.h"
#include "SampleScene/SampleScene.h"
#include <SingleCalculation.h>
#include <Collider/CollisionConfig.h>

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
	const Vector3 INITIAL_POSITION = { .x=0.0f,.y=0.0f,.z=-10.0f };
	worldTransform_.translate_ = INITIAL_POSITION;

	#pragma region 当たり判定について

	collisionType_ = CollisionType::SphereType;

	//半径
	radius_ = 1.0f;

	//AABBのmax部分に加算する縦横高さのサイズ
	upSideSize_ = {1.0f,1.0f,1.0f};

	//AABBのmax部分に加算する縦横高さのサイズ
	downSideSize_ = {1.0f,1.0f,1.0f};

	#pragma endregion


	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY_ATTACK);


	
}

void Player::Update(){

	const float MOVE_SPEED = 0.1f;



	//動けるときだけ加算
	if (isControll_ == true && moveCondition_==PlayerMoveCondition::OnPlayerMove) {
		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, VectorCalculation::Multiply(moveDirection_,MOVE_SPEED));

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

	



	
#ifdef _DEBUG
	//ImGuiにInputUintが無いの不便・・
	int keyQuantity = haveKeyQuantity_;
	int condition = moveCondition_;


	ImGui::Begin("プレイヤー");
	if (ImGui::TreeNode("Condition")) {
		ImGui::InputInt("KeyQuantity", &keyQuantity);
		ImGui::InputInt("HP", &hp_);
		ImGui::Checkbox("acceptDamage_", &acceptDamage_);
		ImGui::Checkbox("isDamage_", &isDamage_);

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
}

