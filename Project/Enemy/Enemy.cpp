#include "Enemy.h"

#include "../Collider/CollisionConfig.h"
#include <VectorCalculation.h>

#include "SpotLight.h"
#include <SingleCalculation.h>
#include <numbers>
#include <ModelManager.h>

void Enemy::Initialize(const uint32_t& modelHandle, const Vector3& position, const Vector3& speed){
	
	//モデルの生成
	model_.reset(Model::Create(modelHandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale = { .x = SCALE_SIZE,.y = SCALE_SIZE ,.z = SCALE_SIZE };
#ifdef _DEBUG
	float DEBUG_SCALE = 1.0f;
	worldTransform_.scale = { .x = DEBUG_SCALE,.y = DEBUG_SCALE ,.z = DEBUG_SCALE };
#endif // _DEBUG

	//座標の代入
	worldTransform_.translate = position;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Spot;

	//生存か死亡
	isAlive_ = true;
	deleteTime_ = 180;
	
	//追跡かどうか
	isTracking_ = false;

	//スピードの初期化
	preSpeed_ = speed;
	speed_ = speed;

	//デフォルトで右方向に向いているようにする
	direction_ = { 1.0f,0.0f,0.0f };

	//状態
	preCondition_ = EnemyCondition::NoneMove;
	condition_ = EnemyCondition::Move;


	
	//攻撃
	attackTime_ = 0;


	//パーティクル
	particle_.reset(Particle3D::Create(ParticleMoveType::Rise));

	//デバッグ用のモデル
	uint32_t debugModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Sphere", "Sphere.obj");

	//攻撃の当たり判定
	attackCollision_ = std::make_unique<EnemyAttackCollision>();
	attackCollision_->Initialize(debugModelHandle);

	//懐中電灯に対する当たり判定
	enemyFlashLightCollision_ = std::make_unique<EnemyFlashLightCollision>();
	enemyFlashLightCollision_->Initialize();

}



void Enemy::Damaged(){


	//懐中電灯用の当たり判定に当たっていたら色が変わっていくよ
	if (enemyFlashLightCollision_->GetIsTouched() == true) {
		const float COLOR_CHANGE_INTERVAL = 0.005f;
		material_.color_.y -= COLOR_CHANGE_INTERVAL;
		material_.color_.z -= COLOR_CHANGE_INTERVAL;

	}

	//0になったら消す
	if (material_.color_.y < 0.0f &&
		material_.color_.z < 0.0f) {
		isAlive_ = false;
	}



}



void Enemy::Update(){
	
	const float SPEED_AMOUNT = 0.05f;
	//StatePatternにしたい！！
	 
	if (isAlive_ == true) {
		//状態遷移
		switch (condition_) {
			//何もしない
		case EnemyCondition::NoneMove:
#ifdef _DEBUG
			ImGui::Begin("None");
			ImGui::End();
#endif // DEBUG

			attackTime_ = 0;
			preTrackingPlayerPosition_ = {};
			preTrackingPosition_ = {};
			speed_ = { 0.0f,0.0f,0.0f };
			break;

			//通常の動き
		case EnemyCondition::Move:
			attackTime_ = 0;

			//通常の動き
			preTrackingPlayerPosition_ = {};
			preTrackingPosition_ = {};
			if (speed_.x != 0.0f ||
				speed_.y != 0.0f ||
				speed_.z != 0.0f) {
				direction_ = VectorCalculation::Normalize(speed_);
			}

			worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, speed_);


			break;

		case EnemyCondition::PreTracking:

			attackTime_ = 0;
			//取得したら追跡
			preTrackingPlayerPosition_ = playerPosition_;
			preTrackingPosition_ = GetWorldPosition();



			//強制的に追跡
			preCondition_ = EnemyCondition::PreTracking;
			condition_ = EnemyCondition::Tracking;

			break;

		case EnemyCondition::Tracking:
			//追跡処理


#ifdef _DEBUG
			ImGui::Begin("Tracking");
			ImGui::End();
#endif // DEBUG



			//向きを求める
			direction_ = VectorCalculation::Subtract(playerPosition_, GetWorldPosition());
			direction_ = VectorCalculation::Normalize(direction_);

			//加算
			Vector3 speedVelocity = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
			worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, speedVelocity);


			break;

			//攻撃
		case EnemyCondition::Attack:
			attackTime_ += 1;


			//2秒の時に攻撃
			if (attackTime_ == 121) {
				//ここで攻撃
				//コライダーが当たっている時だけ通す

				isAttack_ = true;
			}
			else {
				isAttack_ = false;
			}

			//4秒経ったらまた0になる
			if (attackTime_ > 240) {
				attackTime_ = 0;

			}

			break;

		}
	}
	


	

	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(-direction_.z,direction_.x);

	const float ROTATE_OFFSET = -std::numbers::pi_v<float>/2.0f;
	worldTransform_.rotate.y = directionToRotateY + ROTATE_OFFSET;

#ifdef _DEBUG
	const float DEBUG_MODEL_ROTATE_OFFSET = std::numbers::pi_v<float>;
	worldTransform_.rotate.y = directionToRotateY + DEBUG_MODEL_ROTATE_OFFSET;
#endif // _DEBUG


	

	

	//更新
	worldTransform_.Update();
	material_.Update();

	//AABBの計算
	aabb_.max = VectorCalculation::Add(GetWorldPosition(), RADIUS_INTERVAL_);
	aabb_.min = VectorCalculation::Subtract(GetWorldPosition(), RADIUS_INTERVAL_);


	//当たり判定
	//懐中電灯
	enemyFlashLightCollision_->SetEnemyPosition(GetWorldPosition());
	enemyFlashLightCollision_->Update();
	
	//攻撃
	attackCollision_->SetEnemyPosition(GetWorldPosition());
	attackCollision_->SetEnemyDirection(direction_);
	attackCollision_->Update();

	//座標の設定
	particle_->SetTranslate(GetWorldPosition());


#ifdef _DEBUG
	float degreeRotateY = directionToRotateY * (180.0f / std::numbers::pi_v<float>);

	ImGui::Begin("敵");
	ImGui::InputFloat3("方向", &direction_.x);
	ImGui::InputFloat("回転Y", &degreeRotateY);
	ImGui::Checkbox("攻撃", &isAttack_);
	ImGui::InputInt("攻撃時間", &attackTime_);
	
	if (ImGui::TreeNode("状態")) {
		int newCondition = static_cast<int>(condition_);
		int newPreCondition = static_cast<int>(preCondition_);
		ImGui::InputInt("現在", &newCondition);
		ImGui::InputInt("前", &newPreCondition);
		ImGui::TreePop();
	}
	

	ImGui::InputFloat4("色", &material_.color_.x);
	ImGui::InputFloat3("座標", &worldTransform_.translate.x);
	ImGui::InputFloat3("追跡前のプレイヤーの座標", &preTrackingPlayerPosition_.x);
	ImGui::InputFloat3("追跡前の座標", &preTrackingPosition_.x);
	ImGui::InputInt("AliveTive", &deleteTime_);
	ImGui::End();
#endif // _DEBUG

	//ダメージ演出
	Damaged();
}







void Enemy::Draw(const Camera& camera,const SpotLight&spotLight){
#ifdef _DEBUG
	//攻撃
	attackCollision_->Draw(camera, spotLight);
#endif // _DEBUG


	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera,material_, spotLight);
	}
	

	//絶命したらパーティクルの表示
	if (isAlive_ == false) {
		particle_->Draw(camera, material_, spotLight);
	}
}

