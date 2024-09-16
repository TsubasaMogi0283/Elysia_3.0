#include "EnemyManager.h"
#include <Input.h>
#include <cassert>

#include "Player/Player.h"
#include "Stage/ObjectManager/ObjectManager.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>
#include <random>

void EnemyManager::Initialize(uint32_t modelhandle){
	
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
	modelHandle_ = modelhandle;
	assert(modelHandle_ != 0);

	//TLのレベルエディターでやってもいいかも！
	Enemy* enemy1 = new Enemy();
	Vector3 position1 = { 0.0f,0.0f,7.0f };
	enemy1->SetRadius_(ENEMY_SCALE_SIZE_);
	enemy1->Initialize(modelHandle_, position1, { -0.0f,0.0f,-0.01f });
	enemyes_.push_back(enemy1);

		
	//Enemy* enemy2 = new Enemy();
	//Vector3 position2 = { 10.0f,0.0f,18.0f };
	//enemy2->SetRadius_(ENEMY_SCALE_SIZE_);
	//enemy2->Initialize(modelHandle_, position2, { -0.02f,0.0f,0.0f });
	//enemyes_.push_back(enemy2);
	//
	//Enemy* enemy3 = new Enemy();
	//Vector3 position3 = { -10.0f,0.0f,4.0f };
	//enemy3->Initialize(modelHandle_, position3, { 0.01f,0.0f,0.01f });
	//uint32_t condition = EnemyCondition::Move;
	//enemy3->SetCondition(condition);
	//enemy3->SetRadius_(player_->GetRadius());
	//enemyes_.push_back(enemy3);
	//"C:\Lesson\CG\CGGrade3\Ellysia_3.0\Resources\Sample\TD2_Enemy\TD2_Enemy.obj"

	

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
			float defferenceDistance = SingleCalculation::Length(defference);


			//正射影ベクトル
			Vector3 projectDifference = VectorCalculation::Project(defference, enemyDirection);
			float projectDefferenceDistance = SingleCalculation::Length(projectDifference);

			
			//プレイヤーと敵の間隔
			const float PLAYER_ENEMY_INTERVAL = player_->GetRadius() + enemy->GetRadius();
			PLAYER_ENEMY_INTERVAL;


			

			//範囲外になったら通常の動きへ。
			if (projectDefferenceDistance > TRACKING_START_DISTANCE_) {

				

				condition = EnemyCondition::Move;
				enemy->SetCondition(condition);

#ifdef _DEBUG
				ImGui::Begin("ChangeMove"); 
				ImGui::End();
#endif // _DEBUG


			}

			Vector3 normalizedDefference = VectorCalculation::Normalize(defference);
			float dot = SingleCalculation::Dot(normalizedDefference, enemyDirection);

#ifdef _DEBUG
			ImGui::Begin("Dot");
			ImGui::InputFloat("Value", &dot);
			ImGui::End();
#endif // _DEBUG

			
			//設定した値より短くなったら接近開始
			if (condition == EnemyCondition::Move) {


				//ステージの端またはステージオブジェクトに当たったら
				//スピードが反転する
				

				//追跡準備をする。15
				if (projectDefferenceDistance <= TRACKING_START_DISTANCE_&& dot >0.9f) {
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					enemy->SetPreCondition(preCondition);
					enemy->SaveSpeed();
					//状態を記録
					condition = EnemyCondition::PreTracking;
					enemy->SetCondition(condition);
				}
			}
			
			//追跡の時に
			if (condition == EnemyCondition::Tracking) {
				//進行方向にいない場合
				//Moveへ
				if ((projectDefferenceDistance > TRACKING_START_DISTANCE_ ) && (dot <= 0.9f)) {
			
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					enemy->SetPreCondition(preCondition);
					
					//状態を記録
					condition = EnemyCondition::Move;
					enemy->SetCondition(condition);
			
				}
			
				//設定した値より短くなったら攻撃開始
				if (projectDefferenceDistance <= ATTACK_START_DISTANCE_ &&
					MINIMUM_DISTANCE < projectDefferenceDistance) {
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
				//攻撃し終わった後距離が離れていれば通常の動きに戻る
				if ((projectDefferenceDistance >= MINIMUM_DISTANCE)) {
			
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
			ImGui::InputFloat3("Difference", &defference.x);
			ImGui::InputFloat("defferenceDistance", &defferenceDistance);
			ImGui::InputFloat3("projectDifference", &projectDifference.x);
			ImGui::InputFloat("Distance", &projectDefferenceDistance);
			ImGui::InputInt("Condition", &debugCondition);
			ImGui::InputInt("PreCondition", &debugPreCondition);
			ImGui::End();
#endif // _DEBUG
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
}

void EnemyManager::Draw(Camera& camera, SpotLight& spotLight){

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



