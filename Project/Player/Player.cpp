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
	acceptDamage_ = true;
	isDamage_ = false;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const Vector3 INITIAL_POSITION = { .x=0.0f,.y=0.0f,.z=-15.0f };
	worldTransform_.translate = INITIAL_POSITION;

	//半径
	radius_ = 1.0f;

	//AABBのmax部分に加算する縦横高さのサイズ
	upSideSize_ = {1.0f,1.0f,1.0f};

	//AABBのmin部分に加算する縦横高さのサイズ
	downSideSize_ = {1.0f,1.0f,1.0f};


	//通常
	colliderToNormalEnemy_ = std::make_unique<PlayerCollisionToNormalEnemyAttack>();
	colliderToNormalEnemy_->Initialize();


	//強敵
	collisionToStrongEnemy_ = std::make_unique<PlayerCollisionToStrongEnemy>();
	collisionToStrongEnemy_->Initialize();



	//ダメージを受けたかどうか
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
		worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, VectorCalculation::Multiply(moveDirection_, moveSpeed));

	}
	

	//ステージの外には行けないようにする
	//左
	if (worldTransform_.translate.x < stageRect_.leftBack.x + radius_) {
		worldTransform_.translate.x = stageRect_.leftBack.x + radius_;
	}
	//右
	if (worldTransform_.translate.x > stageRect_.rightBack.x - radius_) {
		worldTransform_.translate.x = stageRect_.rightBack.x - radius_;
	}
	//奥
	if (worldTransform_.translate.z > stageRect_.leftBack.z - radius_) {
		worldTransform_.translate.z = stageRect_.leftBack.z - radius_;
	}
	//手前
	if (worldTransform_.translate.z < stageRect_.leftFront.z + radius_) {
		worldTransform_.translate.z = stageRect_.leftFront.z + radius_;
	}

	//ワールドトランスフォームの更新
	worldTransform_.Update();
	Vector3 worldPosition = worldTransform_.GetWorldPosition();


	aabb_.min.x = worldPosition.x - downSideSize_.x;
	aabb_.min.y = worldPosition.y - downSideSize_.y;
	aabb_.min.z = worldPosition.z - downSideSize_.z;

	aabb_.max.x = worldPosition.x + downSideSize_.x;
	aabb_.max.y = worldPosition.y + downSideSize_.y;
	aabb_.max.z = worldPosition.z + downSideSize_.z;

	//通常の敵用の当たり判定の更新
	colliderToNormalEnemy_->SetPlayerPosition(worldPosition);
	colliderToNormalEnemy_->Update();

	//一発アウト用の当たり判定
	collisionToStrongEnemy_->SetPlayerPosition(worldPosition);
	collisionToStrongEnemy_->Update();



	//通常の敵に当たった場合
	if (colliderToNormalEnemy_->GetIsTouch() == true) {
		//体力を1減らす
		--hp_;
	
		//振動させる
		isDameged_ = true;
	}


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
	ImGui::InputFloat3("Transrate", &worldTransform_.translate.x);
	ImGui::InputFloat3("MoveDirection", &moveDirection_.x);
	ImGui::InputInt("moveCondition_", &condition);
	ImGui::End();
#endif

}

void Player::Draw(Camera& camera, Material& material, SpotLight& spotLight){

#ifdef _DEBUG
	//本体の描画
	//1人称視点だからいらないかもね
	model_->Draw(worldTransform_, camera,material,spotLight);

	//通常
	colliderToNormalEnemy_->Draw(camera, material, spotLight);
	//強敵	
	collisionToStrongEnemy_->Draw(camera, material, spotLight);
#endif // _DEBUG


}


Player::~Player() {
	Input::GetInstance()->StopVibration();
}