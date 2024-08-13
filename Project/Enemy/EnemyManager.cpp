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
	Vector3 position1 = { 0.0f,0.0f,4.0f };
	enemy1->SetStageRect(stageRect_);
	enemy1->SetRadius_(ENEMY_SCALE_SIZE_);
	enemy1->Initialize(modelHandle_, position1, { -0.01f,0.0f,0.0f });
	enemyes_.push_back(enemy1);

		
	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { -5.0f,0.0f,10.0f };
	enemy2->SetStageRect(stageRect_);
	enemy2->SetRadius_(ENEMY_SCALE_SIZE_);
	enemy2->Initialize(modelHandle_, position2, { -0.0f,0.0f,-0.0f });
	enemyes_.push_back(enemy2);

	//Enemy* enemy3 = new Enemy();
	//Vector3 position3 = { 5.0f,0.0f,9.0f };
	//enemy3->Initialize(modelHandle_, position3, { -0.01f,0.0f,0.0f });
	//enemy3->SetRadius_(player_->GetRadius());
	//enemyes_.push_back(enemy3);


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
	if (enemyAmount == 1) {
		for (Enemy* enemy : enemyes_) {
			uint32_t condition = enemy->GetCondition();
			Vector3 enemyPosition = enemy->GetWorldPosition();

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
		//昇順
		for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {
			for (std::list<Enemy*>::iterator it2 = std::next(it1); it2 != enemyes_.end(); ++it2) {


				//it1とit2が一致した場合は計算をせずに次のループへ
				if (it1 == it2) {
					continue;
				}

				//AABB
				AABB aabb1 = (*it1)->GetAABB();
				AABB aabb2 = (*it2)->GetAABB();

				//ワールド座標
				Vector3 enemy1Position = (*it1)->GetWorldPosition();
				Vector3 enemy2Position = (*it2)->GetWorldPosition();

				//向き
				Vector3 direction = (*it1)->GetDirection();
				
				//敵同士の差分ベクトル
				Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
				
				//敵同士の距離
				float enemyAndEnemyDistance = sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));
				


				// 正射影ベクトルを求める
				Vector3 projectVector = VectorCalculation::Project(enemyAndEnemyDifference, direction);
				Vector3 differenceEnemyAndProject = VectorCalculation::Subtract(enemyAndEnemyDifference, projectVector);
				float projectDistance = sqrtf(std::powf(differenceEnemyAndProject.x, 2.0f) + std::powf(differenceEnemyAndProject.y, 2.0f) + std::powf(differenceEnemyAndProject.z, 2.0f));

				//内積
				//進行方向の前にいると+
				Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
				float dot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);
				
				//現在の状態
				uint32_t condition = (*it1)->GetCondition();
				
				//前方にいた場合
				if (dot > 0.0f) {
					//衝突判定
					if ((aabb1.min.x < aabb2.max.x && aabb1.max.x > aabb2.min.x) &&
						(aabb1.min.y < aabb2.max.y && aabb1.max.y > aabb2.min.y) &&
						(aabb1.min.z < aabb2.max.z && aabb1.max.z > aabb2.min.z)) {




					}
				}

				

				// 進行方向上にいたら
				if ((ENEMY_SCALE_SIZE_*2.0f > projectDistance) &&
					(condition == EnemyCondition::Move || condition == EnemyCondition::Tracking)) {
				
					#ifdef _DEBUG
					ImGui::Begin("On the direction of progress");
					ImGui::End();
					#endif // _DEBUG

					// 接触していたら
					if ((enemyAndEnemyDistance < ENEMY_SCALE_SIZE_ * 2.0f)&& dot>0.7f) {
						
						#ifdef _DEBUG
						ImGui::Begin("Touch");
						ImGui::End();
						#endif // _DEBUG

						uint32_t newCondition = EnemyCondition::NoneMove;
						(*it1)->SetCondition(newCondition);
						(*it1)->SetPreCondition(condition);
					}
					// 接触していない
					else {

						#ifdef _DEBUG
						ImGui::Begin("NotTouch");
						ImGui::End();
						#endif // _DEBUG

						uint32_t newCondition = EnemyCondition::Move;
						(*it1)->SetCondition(newCondition);
						(*it1)->SetPreCondition(condition);
					}
				}

				//いなかった場合
				if (ENEMY_SCALE_SIZE_ * 2.0f <= projectDistance) {
					uint32_t newCondition = EnemyCondition::Move;
					(*it1)->SetCondition(newCondition);
				}
			
			}
		}

		//降順必要ないかも

		//降順の計算をするのは2体以上いる時だけ
		//必要のない計算は出来るだけしないようにね
		
		//降順
		for (std::list<Enemy*>::iterator it1 = std::prev(enemyes_.end());; --it1) {

			//最初に戻ってきたらbreakでループを抜ける
			if (it1 == enemyes_.begin()) {
				break;
			}

			for (std::list<Enemy*>::iterator it2 = std::prev(it1); ; --it2) {



				//ワールド座標
				Vector3 enemy1Position = (*it1)->GetWorldPosition();
				Vector3 enemy2Position = (*it2)->GetWorldPosition();

				//向き
				Vector3 direction = (*it1)->GetDirection();
				Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
				float enemyAndEnemyDistance = sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));

				// 正射影ベクトルを求める
				Vector3 projectVector = VectorCalculation::Project(enemyAndEnemyDifference, direction);
				Vector3 differenceEnemyAndProject = VectorCalculation::Subtract(enemyAndEnemyDifference, projectVector);
				float projectDistance = sqrtf(std::powf(differenceEnemyAndProject.x, 2.0f) + std::powf(differenceEnemyAndProject.y, 2.0f) + std::powf(differenceEnemyAndProject.z, 2.0f));

				//内積
				//進行方向の前にいると+
				Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
				float dot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);

				//現在の状態
				uint32_t condition = (*it1)->GetCondition();

				// 進行方向上にいたら
				if ((ENEMY_SCALE_SIZE_ * 2.0f > projectDistance) &&
					(condition == EnemyCondition::Move || condition == EnemyCondition::Tracking)) {

#ifdef _DEBUG
					ImGui::Begin("222222On the direction of progress");
					ImGui::End();
#endif // _DEBUG

					// 接触していたら
					if ((enemyAndEnemyDistance < ENEMY_SCALE_SIZE_ * 2.0f) && dot > 0.0f) {

#ifdef _DEBUG
						ImGui::Begin("22222222Touch");
						ImGui::End();
#endif // _DEBUG
						//移動無しに転移
						uint32_t newCondition = EnemyCondition::NoneMove;
						(*it1)->SetPreCondition(condition);
						(*it1)->SetCondition(newCondition);

					}
					// 接触していない
					else {

#ifdef _DEBUG
						ImGui::Begin("222222NotTouch");
						ImGui::End();
#endif // _DEBUG

						uint32_t newCondition = EnemyCondition::Move;
						(*it1)->SetCondition(newCondition);
						(*it1)->SetPreCondition(condition);
					}
				}
				//いなかった場合
				if (ENEMY_SCALE_SIZE_ * 2.0f <= projectDistance) {
					uint32_t newCondition = EnemyCondition::Move;
					(*it1)->SetCondition(newCondition);
					if (condition == EnemyCondition::Move) {

					}
					if (condition == EnemyCondition::Tracking) {
						//uint32_t newCondition = EnemyCondition::Tracking;
						//(*it1)->SetCondition(newCondition);
					}
				}


				if (it2 == enemyes_.begin()) {
					break;
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



