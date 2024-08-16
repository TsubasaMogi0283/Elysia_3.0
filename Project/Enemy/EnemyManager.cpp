#include "EnemyManager.h"
#include <Input.h>
#include <cassert>

#include "Player/Player.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>
#include <random>

void EnemyManager::Initialize(uint32_t modelhandle){
	
	//Playerが空だったら引っかかるようにしている
	assert(player_!=nullptr);

	//Stageの四隅が一つでも同じだったら引っかかるようにしている
	//X軸
	assert((stageRect_.leftBack.x != stageRect_.leftFront.x)||
		(stageRect_.leftBack.x != stageRect_.rightBack.x)||
		(stageRect_.leftBack.x != stageRect_.rightFront.x));

	//Z軸
	assert((stageRect_.leftBack.z != stageRect_.leftFront.z) ||
		(stageRect_.leftBack.z != stageRect_.rightBack.z) ||
		(stageRect_.leftBack.z != stageRect_.rightFront.z));


	//モデルを代入
	modelHandle_ = modelhandle;
	assert(modelHandle_ != 0);

	//TLのレベルエディターでやってもいいかも！
	Enemy* enemy1 = new Enemy();
	Vector3 position1 = { -5.0f,0.0f,4.0f };

	enemy1->SetStageRect(stageRect_);
	enemy1->SetRadius_(ENEMY_SCALE_SIZE_);
	enemy1->Initialize(modelHandle_, position1, { 0.01f,0.0f,0.0f });
	uint32_t condition = EnemyCondition::Move;
	enemy1->SetCondition(condition);
	enemyes_.push_back(enemy1);

		
	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { 0.0f,0.0f,4.0f };

	enemy2->SetStageRect(stageRect_);
	enemy2->SetRadius_(ENEMY_SCALE_SIZE_);
	enemy2->Initialize(modelHandle_, position2, { -0.0f,0.0f,-0.0f });
	enemyes_.push_back(enemy2);

	//Enemy* enemy3 = new Enemy();
	//Vector3 position3 = { 5.0f,0.0f,9.0f };
	//enemy3->Initialize(modelHandle_, position3, { -0.01f,0.0f,0.0f });
	//enemy3->SetRadius_(player_->GetRadius());
	//enemyes_.push_back(enemy3);
	//"C:\Lesson\CG\CGGrade3\Ellysia_3.0\Resources\Sample\TD2_Enemy\TD2_Enemy.obj"

	//モデル
	uint32_t debugModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	

	debugModel_.reset(Model::Create(debugModelHandle));

	debugModelWorldTransform_.Initialize();
	debugModelWorldTransform_.scale_ = { 0.5f,0.5f,0.5f };

	material_.Initialize();
	material_.lightingKinds_ = Spot;

}



void EnemyManager::DeleteEnemy(){
	//敵が生存していなかったら消す
	enemyes_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsAlive() == false) {
			delete enemy;
			return true;
		}
		return false;
	});

}

void EnemyManager::GenarateEnemy() {
	Enemy* enemy = new Enemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribute(-30.0f, 30.0f);
	Vector3 position1 = { distribute(randomEngine),0.0f,distribute(randomEngine) };
	enemy->Initialize(modelHandle_, position1, { 0.0f,0.0f,0.0f });
	enemy->SetRadius_(player_->GetRadius());
	enemyes_.push_back(enemy);

}


void EnemyManager::Update(){


#ifdef _DEBUG
	//Gキーで出す
	if (Input::GetInstance()->IsTriggerKey(DIK_G) == true) {
		GenarateEnemy();
	}

#endif



	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 15.0f;
	TRACKING_START_DISTANCE_;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;
	ATTACK_START_DISTANCE_;

	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	const float ATTACK_DISTANCE_OFFSET = 0.0f;
	float MINIMUM_DISTANCE = player_->GetRadius() +1.0f + ATTACK_DISTANCE_OFFSET;
	MINIMUM_DISTANCE;
	
	
	
	



	for (Enemy* enemy : enemyes_) {
		enemy->SetPlayerPosition(playerPosition);
		enemy->Update();
	}
	

	//現在の敵の数
	uint32_t enemyAmount = static_cast<uint32_t>(enemyes_.size());

	//1体だけの時
	//衝突判定をやる必要が無いからね
	if (enemyAmount == 1) {
		for (Enemy* enemy : enemyes_) {
			uint32_t condition = enemy->GetCondition();
			//Vector3 enemyPosition = enemy->GetWorldPosition();

			//プレイヤーとの距離を求める
			Vector3 difference = VectorCalculation::Subtract(playerPosition, enemy->GetWorldPosition());
			float distance = sqrtf(std::powf(difference.x, 2.0f) + std::powf(difference.y, 2.0f) + std::powf(difference.z, 2.0f));




			//通常時
			if (distance > TRACKING_START_DISTANCE_) {
				condition = EnemyCondition::Move;
				enemy->SetCondition(condition);

			}


			//設定した値より短くなったら接近開始
			if (condition == EnemyCondition::Move) {
				if (distance <= TRACKING_START_DISTANCE_) {
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					enemy->SetPreCondition(preCondition);
					
					//状態を記録
					condition = EnemyCondition::PreTracking;
					enemy->SetCondition(condition);
				}
			}

			//追跡の時に
			if (condition == EnemyCondition::Tracking) {

				//if ((enemy->GetWorldPosition().x< stageRect_.leftBack.x + ENEMY_SCALE_SIZE_*1.0f)|| (enemy->GetWorldPosition().x > stageRect_.rightBack.x - ENEMY_SCALE_SIZE_ * 1.0f)) {
				//	//前回のMove状態を記録
				//	uint32_t preCondition = condition;
				//	enemy->SetPreCondition(preCondition);
				//
				//	//状態を記録
				//	condition = EnemyCondition::Move;
				//	enemy->SetCondition(condition);
				//
				//}
				//if ((enemy->GetWorldPosition().z > stageRect_.leftBack.x - ENEMY_SCALE_SIZE_ * 1.0f) || (enemy->GetWorldPosition().z< stageRect_.leftFront.z + ENEMY_SCALE_SIZE_ * 1.0f)) {
				//	//前回のMove状態を記録
				//	uint32_t preCondition = condition;
				//	enemy->SetPreCondition(preCondition);
				//
				//	//状態を記録
				//	condition = EnemyCondition::Move;
				//	enemy->SetCondition(condition);
				//
				//}

				
				//Moveへ
				if (distance > TRACKING_START_DISTANCE_ ) {
					
					float TRACKING_STOP_BACK_LINE = stageRect_.leftBack.z - ENEMY_SCALE_SIZE_ * 2.0f;
					float TRACKING_STOP_FRONT_LINE = stageRect_.leftFront.z + ENEMY_SCALE_SIZE_ * 2.0f;


					if (enemy->GetWorldPosition().z > TRACKING_STOP_BACK_LINE) {
						enemy->SetPositionZ(TRACKING_STOP_BACK_LINE);

					}
					if (enemy->GetWorldPosition().z < stageRect_.leftFront.z + ENEMY_SCALE_SIZE_ * 2.0f) {
						enemy->SetPositionZ(TRACKING_STOP_FRONT_LINE);

					}

					//前回のMove状態を記録
					uint32_t preCondition = condition;
					enemy->SetPreCondition(preCondition);
					
					//状態を記録
					condition = EnemyCondition::Move;
					enemy->SetCondition(condition);

				}

				//設定した値より短くなったら攻撃開始
				if (distance <= ATTACK_START_DISTANCE_ &&
					MINIMUM_DISTANCE < distance) {
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					enemy->SetPreCondition(preCondition);

					//状態を記録
					condition = EnemyCondition::Attack;
					enemy->SetCondition(condition);

				}

			}

			//攻撃の時に
			if (condition == EnemyCondition::Attack) {
				////攻撃し終わった後距離が離れていれば通常の動きに戻る
				//離れていなければもう一回攻撃
				if ((distance >= ATTACK_START_DISTANCE_)) {

					//前回の記録
					uint32_t preCondition = condition;
					enemy->SetPreCondition(preCondition);

					//状態を記録
					condition = EnemyCondition::Move;
					enemy->SetCondition(condition);
				}



			}


#ifdef _DEBUG

			int32_t debugCondition = static_cast<uint32_t>(condition);
			int32_t debugPreCondition = static_cast<uint32_t>(enemy->GetPreCondition());

			ImGui::Begin("Enemy1"); 
			ImGui::InputFloat("Distance", &distance);
			ImGui::InputInt("Condition", &debugCondition);
			ImGui::InputInt("PreCondition", &debugPreCondition);
			ImGui::End();
#endif // _DEBUG
		}
	}



	//1体より多い時
	if (enemyAmount > 1) {
		

		//イテレーターは同時に始める
		//一致したらcontinueで次のループで

		for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {

			++j;

			//AABB
			aabb[0] = (*it1)->GetAABB();

			enemyPosition[0] = (*it1)->GetWorldPosition();
			//向き
			Vector3 direction = (*it1)->GetDirection();




			for (std::list<Enemy*>::iterator it2 = enemyes_.begin(); it2 != enemyes_.end(); ++it2) {


				++i;


				//std::list<Enemy*>::iterator it2 = std::next(it1); it2 != enemyes_.end(); ++it2

				//it1とit2が一致した場合は計算をせずに次のループへ
				if (it1 == it2) {
					continue;
				}


				aabb[1] = (*it2)->GetAABB();

				//そもそも衝突していないなら計算する必要が無い
				if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
					(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
					(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
					
				}
				else {
					continue;
				}


				//ワールド座標
				enemyPosition[1] = (*it2)->GetWorldPosition();

				
				//敵同士の差分ベクトル
				Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemyPosition[1], enemyPosition[0]);
				
				//内積
				//進行方向の前にいると+
				Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
				dot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);
				
			}




			//現在の状態
			uint32_t condition = (*it1)->GetCondition();

			//前方にいた場合
			if (dot > 0.0f) {
				if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
					(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
					(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
					uint32_t newCondition = EnemyCondition::NoneMove;

					//前の状態を保存
					(*it1)->SetPreCondition(condition);
					//スピードの保存
					//(*it1)->SaveSpeed();
					//状態の変更
					(*it1)->SetCondition(newCondition);
					//(*it1)->TakeOutSpeed();
					continue;
				}
				
			}



		}


		
		
	}
	



	material_.Update();
#ifdef _DEBUG
	debugModelWorldTransform_.Update();

#endif // _DEBUG

	
	

}

void EnemyManager::Draw(Camera& camera, SpotLight& spotLight){
#ifdef _DEBUG
	debugModel_->Draw(debugModelWorldTransform_, camera, material_, spotLight);

#endif // _DEBUG


	//描画
	for (Enemy* enemy : enemyes_) {
		enemy->Draw(camera,spotLight);
	}

}

EnemyManager::~EnemyManager(){
	for (Enemy* enemy : enemyes_) {
		delete enemy;
	}


}



