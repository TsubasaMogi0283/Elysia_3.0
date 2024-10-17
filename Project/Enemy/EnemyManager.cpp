#include "EnemyManager.h"
#include <Input.h>
#include <cassert>

#include "Player/Player.h"
#include "Stage/ObjectManager/ObjectManager.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>
#include <random>

void EnemyManager::Initialize(uint32_t& normalEnemyModel, uint32_t& strongEnemyModel){
	
	//空だったら引っかかるようにしている
	assert(player_!=nullptr);
	assert(objectManager_ != nullptr);

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
	normalEnemyModelHandle_ = normalEnemyModel;
	strongEnemyModelHandle_ = strongEnemyModel;

	////TLのレベルエディターでやってもいいかも！
	Enemy* enemy1 = new Enemy();
	Vector3 position1 = { 0.0f,0.0f,21.0f };
	enemy1->SetRadius_(ENEMY_SCALE_SIZE_);
	enemy1->Initialize(normalEnemyModelHandle_, position1, { -0.0f,0.0f,0.01f });
	enemyes_.push_back(enemy1);

		
	//Enemy* enemy2 = new Enemy();
	//Vector3 position2 = { -20.0f,0.0f,10.0f };
	//enemy2->SetRadius_(ENEMY_SCALE_SIZE_);
	//enemy2->Initialize(normalEnemyModelHandle_, position2, { 0.01f,0.0f,0.0f });
	//enemyes_.push_back(enemy2);
	//
	//Enemy* enemy3 = new Enemy();
	//Vector3 position3 = { -10.0f,0.0f,4.0f };
	//enemy3->Initialize(normalEnemyModelHandle_, position3, { 0.01f,0.0f,0.01f });
	//uint32_t condition = EnemyCondition::Move;
	//enemy3->SetCondition(condition);
	//enemy3->SetRadius_(player_->GetRadius());
	//enemyes_.push_back(enemy3);
	////"C:\Lesson\CG\CGGrade3\Ellysia_3.0\Resources\Sample\TD2_Enemy\TD2_Enemy.obj"
	//
	//生成
	//StrongEnemy* enemy = new StrongEnemy();
	//std::random_device seedGenerator;
	//std::mt19937 randomEngine(seedGenerator());
	//
	////位置を決める
	//std::uniform_real_distribution<float> positionDistribute(stageRect_.leftBack.x, stageRect_.rightBack.x);
	//Vector3 position = { positionDistribute(randomEngine),0.0f,positionDistribute(randomEngine) };
	//
	//
	////位置を決める
	//std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	//Vector3 speed = { speedDistribute(randomEngine),0.0f,speedDistribute(randomEngine) };
	//
	//position = { -4.0f,0.0f,5.0f };
	//speed = { 0.01f,0.0f,-0.03f };
	//
	//
	//enemy->Initialize(strongEnemyModelHandle_, position, speed);
	//enemy->SetRadius_(player_->GetRadius());
	//strongEnemyes_.push_back(enemy);


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
	enemy->Initialize(normalEnemyModelHandle_, position1, { 0.0f,0.0f,0.0f });
	enemy->SetRadius_(player_->GetRadius());
	enemyes_.push_back(enemy);
}


void EnemyManager::GenarateStrongEnemy(){
	//生成
	StrongEnemy* enemy = new StrongEnemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	//位置を決める
	std::uniform_real_distribution<float> positionDistribute(stageRect_.leftBack.x, stageRect_.rightBack.x);
	Vector3 position = { positionDistribute(randomEngine),0.0f,positionDistribute(randomEngine) };
	

	//位置を決める
	std::uniform_real_distribution<float> speedDistribute(-1.0f, 1.0f);
	Vector3 speed = { speedDistribute(randomEngine),0.0f,speedDistribute(randomEngine) };


#ifdef _DEBUG
	position = { -4.0f,0.0f,5.0f };
	speed = { -0.01f,0.0f,0.03f };
#endif // _DEBUG



	enemy->Initialize(strongEnemyModelHandle_, position, speed);
	enemy->SetRadius_(player_->GetRadius());
	strongEnemyes_.push_back(enemy);
}

void EnemyManager::Update(){
#ifdef _DEBUG
	//Gキーで出す
	if (Input::GetInstance()->IsTriggerKey(DIK_G) == true) {
		GenarateStrongEnemy();
	}

#endif

	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 15.0f;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;

	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	const float ATTACK_DISTANCE_OFFSET = 0.0f;
	float MINIMUM_DISTANCE = player_->GetRadius() +1.0f + ATTACK_DISTANCE_OFFSET;
	
	
	//敵はポリモーフィズムでやった方がよさそう

	//全ての敵
	for (Enemy* enemy : enemyes_) {
		//プレイヤーの位置を設定
		//使っているのはPreTrackingしかないので
		if (enemy->GetCondition() == EnemyCondition::PreTracking) {
			enemy->SetPlayerPosition(playerPosition);

		}
		
		//更新
		enemy->Update();

		//AABB
		AABB enemyAABB = enemy->GetAABB();

		//移動中の時
		if (enemy->GetCondition() == EnemyCondition::Move) {

			#pragma region ステージの端に行ったら反転
			//X
			if ((enemyAABB.min.x < stageRect_.leftBack.x) || (enemyAABB.max.x > stageRect_.rightBack.x)) {
				enemy->InvertSpeedX();
			}
			//Z
			if ((enemyAABB.min.z < stageRect_.leftFront.z) || (enemyAABB.max.z > stageRect_.leftBack.z)) {
				enemy->InvertSpeedZ();
			}
			#pragma endregion

			#pragma region ステージオブジェクト

			//仮置きのステージオブジェクト
			std::list<StageObject*>stageObjects=objectManager_->GetStageObjets();
			for (StageObject* stageObject : stageObjects) {

				//AABBを取得
				AABB objectAABB = stageObject->GetAABB();

				//位置を取得
				Vector3 objectPosition = stageObject->GetWorldPosition();

				
				
				//お互いのAABBが接触している場合
				if (((enemyAABB.max.x > objectAABB.min.x) && (enemyAABB.min.x < objectAABB.max.x)) &&
					((enemyAABB.max.z > objectAABB.min.z) && (enemyAABB.min.z < objectAABB.max.z))) {
					//オブジェクトとの差分ベクトル
					Vector3 defference = VectorCalculation::Subtract(objectPosition, enemy->GetWorldPosition());
					Vector3 normalizedDefference = VectorCalculation::Normalize(defference);


					//敵の向いている方向
					Vector3 enemyDirection = enemy->GetDirection();



					//前にある場合だけ計算
					float dot = SingleCalculation::Dot(enemyDirection, normalizedDefference);

					//進行方向上にあるときだけ計算する
					if (dot > 0.0f) {

						//差分ベクトルのXとZの大きさを比べ
						//値が大きい方で反転させる
						float defferenceValueX = std::abs(defference.x);
						float defferenceValueZ = std::abs(defference.z);


						if (defferenceValueX >= defferenceValueZ) {
							enemy->InvertSpeedX();
						}
						else {
							enemy->InvertSpeedZ();
						}



#ifdef _DEBUG
						ImGui::Begin("DemoObjectEnemy");
						ImGui::InputFloat("Dot", &dot);
						ImGui::InputFloat3("defference", &defference.x);
						ImGui::InputFloat("defferenceValueX", &defferenceValueX);
						ImGui::InputFloat("defferenceValueZ", &defferenceValueZ);
						ImGui::End();
#endif // _DEBUG




					}
				}

			}

			#pragma endregion


		}


		if (enemy->GetCondition() == EnemyCondition::Tracking) {
		
			//仮置きのステージオブジェクト
			std::list<StageObject*>stageObjects = objectManager_->GetStageObjets();
			for (StageObject* stageObject : stageObjects) {

				//AABBを取得
				AABB objectAABB = stageObject->GetAABB();

				//位置を取得
				Vector3 objectPosition = stageObject->GetWorldPosition();



				//お互いのAABBが接触している場合
				if (((enemyAABB.max.x > objectAABB.min.x) && (enemyAABB.min.x < objectAABB.max.x)) &&
					((enemyAABB.max.z > objectAABB.min.z) && (enemyAABB.min.z < objectAABB.max.z))) {
					//オブジェクトとの差分ベクトル
					Vector3 defference = VectorCalculation::Subtract(objectPosition, enemy->GetWorldPosition());
					Vector3 normalizedDefference = VectorCalculation::Normalize(defference);


					//敵の向いている方向
					Vector3 enemyDirection = enemy->GetDirection();



					//前にある場合だけ計算
					float dot = SingleCalculation::Dot(enemyDirection, normalizedDefference);

					//進行方向上にあるときだけ計算する
					if (dot > 0.0f) {

						//差分ベクトルのXとZの大きさを比べ
						//値が大きい方で反転させる
						float defferenceValueX = std::abs(defference.x);
						float defferenceValueZ = std::abs(defference.z);


						if (defferenceValueX >= defferenceValueZ) {
							enemy->InvertSpeedX();
						}
						else {
							enemy->InvertSpeedZ();
						}



#ifdef _DEBUG
						ImGui::Begin("DemoObjectEnemy");
						ImGui::InputFloat("Dot", &dot);
						ImGui::InputFloat3("defference", &defference.x);
						ImGui::InputFloat("defferenceValueX", &defferenceValueX);
						ImGui::InputFloat("defferenceValueZ", &defferenceValueZ);
						ImGui::End();
#endif // _DEBUG




					}
				}
			}
		}


	}
	
	//現在の敵の数
	uint32_t enemyAmount = static_cast<uint32_t>(enemyes_.size());

	//1体だけの時
	//衝突判定をやる必要が無いからね
	if (enemyAmount == 1u) {
		for (Enemy* enemy : enemyes_) {
			uint32_t condition = enemy->GetCondition();


			//向き
			Vector3 enemyDirection = enemy->GetDirection();

			//それぞれのAABB
			AABB playerAABB = player_->GetAABB();
			AABB enemyAABB = enemy->GetAABB();


			//プレイヤーと敵の差分ベクトル
			//対象にするものがプレイヤーなのでプレイヤーから引いてね
			Vector3 defference = VectorCalculation::Subtract(playerPosition, enemy->GetWorldPosition());
			//距離
			float defferenceDistance = SingleCalculation::Length(defference);


			//正射影ベクトル
			Vector3 projectDifference = VectorCalculation::Project(defference, enemyDirection);

			


			Vector3 normalizedDefference = VectorCalculation::Normalize(defference);
			float dot = SingleCalculation::Dot(normalizedDefference, enemyDirection);

			

			if (dot > 0.8f) {
				if ((defferenceDistance < TRACKING_START_DISTANCE_ && defferenceDistance>ATTACK_START_DISTANCE_)
					&& condition == EnemyCondition::Move) {

					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::PreTracking;
					enemy->SetPlayerPosition(playerPosition);
					enemy->SetCondition(newCondition);
				}

				//追跡している時
				if (defferenceDistance <= ATTACK_START_DISTANCE_ && condition == EnemyCondition::Tracking) {
					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::Attack;
					enemy->SetCondition(newCondition);
				}
				//攻撃中にプレイヤーが離れた時
				if (defferenceDistance > ATTACK_START_DISTANCE_ && condition == EnemyCondition::Attack) {
					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::Move;
					enemy->SetCondition(newCondition);
				}
				

			}
			//前方にいなければ強制的にMove
			else {
				//前の状態を保存
				enemy->SetPreCondition(condition);
				//現在の状態を保存
				uint32_t newCondition = EnemyCondition::Move;
				enemy->SetCondition(newCondition);
			}

		}
	}

	//1体より多い時
	if (enemyAmount > 1u) {
		for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {

			//AABB
			aabb[0] = (*it1)->GetAABB();
			
			enemyPosition[0] = (*it1)->GetWorldPosition();
			//向き
			Vector3 direction = (*it1)->GetDirection();

			//内積
			float dot = 0.0f;

			for (std::list<Enemy*>::iterator it2 = enemyes_.begin(); it2 != enemyes_.end(); ++it2) {

				//it1とit2が一致した場合は計算をせずに次のループへ
				if (it1 == it2) {
					continue;
				}

				//2体目のAABBを取得
				aabb[1] = (*it2)->GetAABB();

				//接触している場合
				if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
					(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
					(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
					//ワールド座標
					enemyPosition[1] = (*it2)->GetWorldPosition();


					//敵同士の差分ベクトル
					Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemyPosition[1], enemyPosition[0]);

					//内積
					//進行方向の前にいると+
					Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
					dot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);


					break;
				}


				
			}

			//現在の状態
			uint32_t condition = (*it1)->GetCondition();

			//プレイヤーとの距離
			Vector3 playerEnemyDifference = VectorCalculation::Subtract(playerPosition, (*it1)->GetWorldPosition());
			float playerEnemyDistance = SingleCalculation::Length(playerEnemyDifference);


			//前方にいた場合
			if (dot > 0.0f && condition == EnemyCondition::Move) {
				//接触した場合
				//止まる
				if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
					(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
					(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
					uint32_t newCondition = EnemyCondition::NoneMove;

					//前の状態を保存
					(*it1)->SetPreCondition(condition);
					//状態の変更
					(*it1)->SetCondition(newCondition);
					
					continue;
				}
				//接触していない場合
				else {
					uint32_t newCondition = EnemyCondition::Move;

					//前の状態を保存
					(*it1)->SetPreCondition(condition);
					//状態の変更
					(*it1)->SetCondition(newCondition);
				}
			}
			//前方にいない場合
			else {
				if (playerEnemyDistance > TRACKING_START_DISTANCE_) {
					uint32_t newCondition = EnemyCondition::Move;

					//前の状態を保存
					(*it1)->SetPreCondition(condition);
					//スピードの保存
					//(*it1)->SaveSpeed();
					//状態の変更
					(*it1)->SetCondition(newCondition);
				}


				//設定した値より短くなったら接近開始
				if (condition == EnemyCondition::Move) {
					if (playerEnemyDistance <= TRACKING_START_DISTANCE_) {
						//前回のMove状態を記録
						uint32_t preCondition = condition;
						(*it1)->SetPreCondition(preCondition);

						//状態を記録
						condition = EnemyCondition::PreTracking;
						(*it1)->SetCondition(condition);
					}
				}

				//追跡の時に
				if (condition == EnemyCondition::Tracking) {
					//Moveへ
					if (playerEnemyDistance > TRACKING_START_DISTANCE_) {
						//前回のMove状態を記録
						uint32_t preCondition = condition;
						(*it1)->SetPreCondition(preCondition);

						//状態を記録
						condition = EnemyCondition::Move;
						(*it1)->SetCondition(condition);

					}

					//設定した値より短くなったら攻撃開始
					if (playerEnemyDistance <= ATTACK_START_DISTANCE_ &&
						MINIMUM_DISTANCE < playerEnemyDistance) {
						//前回のMove状態を記録
						uint32_t preCondition = condition;
						(*it1)->SetPreCondition(preCondition);

						//状態を記録
						condition = EnemyCondition::Attack;
						(*it1)->SetCondition(condition);

					}
				}
			}
		}
	}




	for (StrongEnemy* strongEnemy : strongEnemyes_) {
		//一発アウトの敵の更新
		strongEnemy->Update();
		//プレイヤーの座標を設定
		strongEnemy->SetPlayerPosition(playerPosition);

		//AABBの取得
		AABB strongEnemyAABB = strongEnemy->GetAABB();


		//追跡開始の距離
		const float TRACKING_START_DISTANCE = 30.0f;

		//距離を求める
		Vector3 playerStrongEnemyDifference = VectorCalculation::Subtract(playerPosition, strongEnemy->GetWorldPosition());
		float playerStrongEnemyDistance = SingleCalculation::Length(playerStrongEnemyDifference);
		

#ifdef _DEBUG
		ImGui::Begin("強い敵");
		ImGui::InputFloat3("プレイヤーとの差分", &playerStrongEnemyDifference.x);
		ImGui::InputFloat("プレイヤーとの距離", &playerStrongEnemyDistance);
		ImGui::End();
#endif // _DEBUG



		//設定した距離より小さくなると追跡
		if (playerStrongEnemyDistance <= TRACKING_START_DISTANCE) {
			
			//追跡に移行
			uint32_t newCondition = EnemyCondition::Tracking;
			strongEnemy->SetCondition(newCondition);
		}
		else {

			//通常の動きに移行
			uint32_t newCondition = EnemyCondition::Move;
			strongEnemy->SetCondition(newCondition);
		}



#pragma region ステージの端に行ったら反転
		//X
		if ((strongEnemyAABB.min.x < stageRect_.leftBack.x) || 
			(strongEnemyAABB.max.x > stageRect_.rightBack.x)) {
			strongEnemy->InvertSpeedX();
		}
		//Z
		if ((strongEnemyAABB.min.z < stageRect_.leftFront.z) || 
			(strongEnemyAABB.max.z > stageRect_.leftBack.z)) {
			strongEnemy->InvertSpeedZ();
		}
#pragma endregion

	}



}

void EnemyManager::Draw(Camera& camera, SpotLight& spotLight){

	//通常
	for (Enemy* enemy : enemyes_) {
		enemy->Draw(camera,spotLight);
	}

	//一発アウト
	for (StrongEnemy* strongEnemy : strongEnemyes_) {
		strongEnemy->Draw(camera, spotLight);
	}

}

EnemyManager::~EnemyManager(){
	//通常
	for (Enemy* enemy : enemyes_) {
		delete enemy;
	}

	//一発アウト
	for (StrongEnemy* strongEnemy : strongEnemyes_) {
		delete strongEnemy;
	}
}



