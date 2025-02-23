#include "NormalEnemy.h"


#include "CollisionConfig.h"
#include <VectorCalculation.h>

#include "SpotLight.h"
#include <SingleCalculation.h>
#include <numbers>
#include <ModelManager.h>
#include "GlobalVariables.h"

NormalEnemy::NormalEnemy() {
	//インスタンスの取得
	//グローバル変数クラス
	globalVariables_ = Ellysia::GlobalVariables::GetInstance();
}

void NormalEnemy::Initialize(const uint32_t& modelHandle, const Vector3& position, const Vector3& speed) {

	//モデルの生成
	model_.reset(Ellysia::Model::Create(modelHandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//スケールサイズ
	const float SCALE_SIZE = 10.0f;
	worldTransform_.scale = { .x = SCALE_SIZE,.y = SCALE_SIZE ,.z = SCALE_SIZE };
#ifdef _DEBUG
	float DEBUG_SCALE = 1.0f;
	worldTransform_.scale = { .x = DEBUG_SCALE,.y = DEBUG_SCALE ,.z = DEBUG_SCALE };
#endif // _DEBUG

	//座標の代入
	worldTransform_.translate = position;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = SpotLighting;
	//パーティクル
	particleMaterial_.Initialize();
	particleMaterial_.lightingKinds_ = NoneLighting;


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



	//デバッグ用のモデル
	debugModelHandle = Ellysia::ModelManager::GetInstance()->LoadModelFile("Resources/Model/Sample/Sphere", "Sphere.obj");

	//攻撃の当たり判定
	attackCollision_ = std::make_unique<EnemyAttackCollision>();
	attackCollision_->Initialize(debugModelHandle);

	//懐中電灯に対する当たり判定
	enemyFlashLightCollision_ = std::make_unique<EnemyFlashLightCollision>();
	enemyFlashLightCollision_->Initialize();


	//グローバル変数
	globalVariables_->CreateGroup(GROUNP_NAME_);
	//振動
	globalVariables_->AddItem(GROUNP_NAME_, "ShakeOffset", shakeOffset_);
	shakeOffset_ = globalVariables_->GetFloatValue(GROUNP_NAME_, "ShakeOffset");
}

void NormalEnemy::Update() {

	//0から始める
	const uint32_t RESTART_TIME = 0u;
	//スピードの量
	const float SPEED_AMOUNT_ = 0.05f;
	
	//生存している時だけ行動するよ
	if (isAlive_ == true) {
		//状態遷移
		switch (condition_) {
		case EnemyCondition::NoneMove:
			//何もしない

			//ここの全ての値が0
			attackTime_ = RESTART_TIME;
			preTrackingPlayerPosition_ = {};
			preTrackingPosition_ = {};
			speed_ = { .x = 0.0f,.y = 0.0f,.z = 0.0f };

			break;


		case EnemyCondition::Move:
			attackTime_ = RESTART_TIME;

			//通常の動き
			preTrackingPlayerPosition_ = {};
			preTrackingPosition_ = {};

			//正規化
			if (speed_.x != 0.0f ||
				speed_.y != 0.0f ||
				speed_.z != 0.0f) {
				direction_ = VectorCalculation::Normalize(speed_);
			}

			//加算
			worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, speed_);


			break;

		case EnemyCondition::PreTracking:


			//取得したら追跡
			preTrackingPlayerPosition_ = playerPosition_;
			preTrackingPosition_ = GetWorldPosition();



			//強制的に追跡に移行
			preCondition_ = EnemyCondition::PreTracking;
			condition_ = EnemyCondition::Tracking;

			break;

		case EnemyCondition::Tracking:
			//追跡処理
			//向きを求める
			direction_ = VectorCalculation::Subtract(playerPosition_, GetWorldPosition());
			//正規化
			direction_ = VectorCalculation::Normalize(direction_);


			//スピードの計算
			Vector3 speed = VectorCalculation::Multiply(direction_, SPEED_AMOUNT_);
			//加算
			worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, speed);


			break;

			//攻撃
		case EnemyCondition::Attack:
			//増える値
			const uint32_t TIME_INCREASE_VALUE = 1;
			//時間が増えていく
			attackTime_ += TIME_INCREASE_VALUE;

			//1秒の時に攻撃
			const uint32_t JUST_ATTACK_TIME = 60;
			if (attackTime_ == JUST_ATTACK_TIME) {
				//ここで攻撃
				//コライダーが当たっている時だけ通す
				isAttack_ = true;

			}
			else {
				//攻撃しない
				isAttack_ = false;
			}

			//攻撃するときの時間×3の時にまた最初からに戻る
			if (attackTime_ > JUST_ATTACK_TIME * 3) {
				attackTime_ = RESTART_TIME;
			}

			break;

		}
	}

	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(-direction_.z, direction_.x);
	//回転のオフセット
	//元々のモデルの回転が変だったのでこれを足している
	const float ROTATE_OFFSET = -std::numbers::pi_v<float> / 2.0f;
	worldTransform_.rotate.y = directionToRotateY + ROTATE_OFFSET;

#ifdef _DEBUG
	const float DEBUG_MODEL_ROTATE_OFFSET = std::numbers::pi_v<float>;
	worldTransform_.rotate.y = directionToRotateY + DEBUG_MODEL_ROTATE_OFFSET;
#endif // _DEBUG

	//ワールドトランスフォームの更新
	worldTransform_.Update();
	//マテリアルの更新
	material_.Update();
	particleMaterial_.Update();


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


	if (isAttack_ == true) {
		attackCollision_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	}
	else {
		attackCollision_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	//ダメージ演出
	Damaged();

	//死亡したらパーティクルを出して消える
	if (isAlive_ == false) {
		Dead();
	}

	//ImGui
	DisplayImGui();
}

void NormalEnemy::Draw(const Camera& camera, const SpotLight& spotLight) {
#ifdef _DEBUG
	//攻撃
	attackCollision_->Draw(camera, spotLight);

#endif // _DEBUG

	//本体
	model_->Draw(worldTransform_, camera, material_, spotLight);


	if (particle_ != nullptr) {
		particle_->Draw(camera, particleMaterial_);
	}
}

NormalEnemy::~NormalEnemy() {
	globalVariables_->SaveFile(GROUNP_NAME_);
}

void NormalEnemy::Damaged() {

	//ライトの強さによってダメージ量を調整したい


	//懐中電灯用の当たり判定に当たっていたら色が赤に変わっていくよ
	if (enemyFlashLightCollision_->GetIsTouched() == true) {
		const float COLOR_CHANGE_INTERVAL = 0.005f;
		material_.color_.y -= COLOR_CHANGE_INTERVAL;
		material_.color_.z -= COLOR_CHANGE_INTERVAL;

		//生存している時だけ振動
		if (isAlive_ == true) {
			//振動演出
			//体力減っていくと同時に振動幅が大きくなっていくよ
			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());
			std::uniform_real_distribution<float> distribute(-1.0f, 1.0f);


			//現在の座標に加える
			worldTransform_.translate.x += distribute(randomEngine) * (1.0f - material_.color_.y) * shakeOffset_;
			worldTransform_.translate.z += distribute(randomEngine) * (1.0f - material_.color_.y) * shakeOffset_;
		}


	}

	//0になったら絶命
	if (material_.color_.y < 0.0f &&
		material_.color_.z < 0.0f) {
		isAlive_ = false;
	}
}

void NormalEnemy::Dead() {
	//消えていくよ
	const float DELETE_INTERVAL = 0.01f;
	material_.color_.w -= DELETE_INTERVAL;

	//縮小
	const float SCALE_DOWN_VALUE = -0.1f;
	worldTransform_.scale.x += SCALE_DOWN_VALUE;
	worldTransform_.scale.y += SCALE_DOWN_VALUE;
	worldTransform_.scale.z += SCALE_DOWN_VALUE;

	if (worldTransform_.scale.z < 0.0f) {
		worldTransform_.scale.x = 0.0f;
		worldTransform_.scale.y = 0.0f;
		worldTransform_.scale.z = 0.0f;

	}

	//生成
	if (particle_ == nullptr) {
		//生成
		particle_ = std::move(Ellysia::Particle3D::Create(Rise));
		//パーティクルの細かい設定
		particle_->SetTranslate(GetWorldPosition());
		const float SCALE_SIZE = 20.0f;
		particle_->SetScale({ .x = SCALE_SIZE,.y = SCALE_SIZE,.z = SCALE_SIZE });
		particle_->SetCount(20u);
		particle_->SetIsReleaseOnceMode(true);
		particle_->SetIsToTransparent(true);
	}

	//全て消えたら、消えたかどうかのフラグがたつ
	if (particle_->GetIsAllInvisible() == true) {
		isDeleted_ = true;
	}

}

void NormalEnemy::DisplayImGui() {
#ifdef _DEBUG

	ImGui::Begin("敵");
	ImGui::InputFloat3("方向", &direction_.x);
	ImGui::Checkbox("攻撃", &isAttack_);
	ImGui::Checkbox("生存", &isAlive_);
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


}


