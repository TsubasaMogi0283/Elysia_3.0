#include "EnemyManager.h"
#include <Input.h>
#include <cassert>

#include "Player/Player.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>

void EnemyManager::Initialize(uint32_t modelhandle){
	
	//Playerが空だったら引っかかるようにしている
	assert(player_!=nullptr);
	modelHandle_ = modelhandle;


	//TLのレベルエディターでやってもいいかも！
	Enemy* enemy1 = new Enemy();
	Vector3 position1 = { 5.0f,0.0f,9.0f };
	enemy1->Initialize(modelHandle_, position1, { 0.0f,0.0f,0.00f });
	enemy1->SetRadius_(player_->GetRadius());
	enemyes_.push_back(enemy1);

	z = position1.z;

	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { 1.0f,0.0f,9.0f };
	enemy2->Initialize(modelHandle_, position2, { 0.01f,0.0f,-0.0f });
	enemy2->SetRadius_(player_->GetRadius());
	enemyes_.push_back(enemy2);
	z2 = position2.z;

	//Enemy* enemy3 = new Enemy();
	//Vector3 position3 = { 0.0f,0.0f,9.0f };
	//enemy3->Initialize(modelHandle_, position3, { 0.00f,0.0f,0.0f });
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
	Vector3 position1 = { 4.0f,0.0f,10.0f };
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
	const float TRACKING_START_DISTANCE_ = 10.0f;
	TRACKING_START_DISTANCE_;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;
	ATTACK_START_DISTANCE_;

	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	const float ATTACK_DISTANCE_OFFSET = 0.0f;
	float MINIMUM_DISTANCE = player_->GetRadius() +1.0f + ATTACK_DISTANCE_OFFSET;
	MINIMUM_DISTANCE;
	//Vector3 difference = VectorCalculation::Subtract(playerPosition, enemy1->GetWorldPosition());
	//float distance = sqrtf(std::powf(difference.x, 2.0f) + std::powf(difference.y, 2.0f) + std::powf(difference.z, 2.0f));
	//distance;
	
	//通常時
	//if (distance > TRACKING_START_DISTANCE_) {
	//	condition = EnemyCondition::Move;
	//	enemy1->SetCondition(condition);
	//
	//}
	//
	////移動
	//if (condition == EnemyCondition::Move) {
	//	
	//	//設定した値より短くなったら接近開始
	//	if (distance <= TRACKING_START_DISTANCE_) {
	//		condition = EnemyCondition::PreTracking;
	//		enemy1->SetCondition(condition);
	//
	//	}
	//}
	

	for (Enemy* enemy : enemyes_) {
		enemy->SetPlayerPosition(playerPosition);
		enemy->Update();
	}
	


	//空じゃない時判定を取る
	if (enemyes_.empty() == false) {
		//昇順
		for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {
			for (std::list<Enemy*>::iterator it2 = std::next(it1); it2 != enemyes_.end(); ++it2) {

				//ワールド座標
				Vector3 enemy1Position = (*it1)->GetWorldPosition();
				Vector3 enemy2Position = (*it2)->GetWorldPosition();
				//半径
				float enemyRadius1 = (*it1)->GetRadius();
				float enemyRadius2 = (*it2)->GetRadius();

				//向き
				Vector3 direction = (*it1)->GetDirection();
				Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
				float enemyAndEnemyDistance = sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));

				(*it1)->SetPositionZ(z);
				//(*it2)->SetPositionZ(z);
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
				if ((enemyRadius1 + enemyRadius2 > projectDistance) &&
					(condition == EnemyCondition::Move || condition == EnemyCondition::Tracking)) {
				
				#ifdef _DEBUG
				ImGui::Begin("On the direction of progress");
				ImGui::End();
				#endif // _DEBUG

				// 接触していたら
				if ((enemyAndEnemyDistance < enemyRadius1 + enemyRadius2)&& dot>0.0f) {
					
					#ifdef _DEBUG
					ImGui::Begin("Touch");
					ImGui::End();
					#endif // _DEBUG

					uint32_t newCondition = EnemyCondition::NoneMove;
					(*it1)->SetCondition(newCondition);
				}
				// 接触していない
				else {

					#ifdef _DEBUG
					ImGui::Begin("NotTouch");
					ImGui::End();
					#endif // _DEBUG

					uint32_t newCondition = EnemyCondition::Move;
					(*it1)->SetCondition(newCondition);
				}
			}
				//いなかった場合
				if (enemyRadius1 + enemyRadius2 <= projectDistance) {
					uint32_t newCondition = EnemyCondition::Move;
					(*it1)->SetCondition(newCondition);
					if (condition == EnemyCondition::Move) {
						
					}
					if (condition == EnemyCondition::Tracking) {
						//uint32_t newCondition = EnemyCondition::Tracking;
						//(*it1)->SetCondition(newCondition);
					}
				}
			
			}
		}

	//降順
	
		for (std::list<Enemy*>::iterator it1 = std::prev(enemyes_.end());; --it1) {

			if (it1 == enemyes_.begin()) {
				break;
			}

			for (std::list<Enemy*>::iterator it2 = std::prev(it1); ; --it2) {

#ifdef _DEBUG
				ImGui::Begin("aaaaaa");
				ImGui::End();
#endif // _DEBUG


				//ワールド座標
				Vector3 enemy1Position = (*it1)->GetWorldPosition();
				Vector3 enemy2Position = (*it2)->GetWorldPosition();
				//半径
				float enemyRadius1 = (*it1)->GetRadius();
				float enemyRadius2 = (*it2)->GetRadius();

				//向き
				Vector3 direction = (*it1)->GetDirection();
				Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
				float enemyAndEnemyDistance = sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));

				(*it2)->SetPositionZ(z2);
				//(*it2)->SetPositionZ(z);
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
				if ((enemyRadius1 + enemyRadius2 > projectDistance) &&
					(condition == EnemyCondition::Move || condition == EnemyCondition::Tracking)) {

#ifdef _DEBUG
					ImGui::Begin("On the direction of progress");
					ImGui::End();
#endif // _DEBUG

					// 接触していたら
					if ((enemyAndEnemyDistance < enemyRadius1 + enemyRadius2) && dot > 0.0f) {

#ifdef _DEBUG
						ImGui::Begin("Touch");
						ImGui::End();
#endif // _DEBUG

						uint32_t newCondition = EnemyCondition::NoneMove;
						(*it1)->SetCondition(newCondition);
					}
					// 接触していない
					else {

#ifdef _DEBUG
						ImGui::Begin("NotTouch");
						ImGui::End();
#endif // _DEBUG

						uint32_t newCondition = EnemyCondition::Move;
						(*it1)->SetCondition(newCondition);
					}
				}
				//いなかった場合
				if (enemyRadius1 + enemyRadius2 <= projectDistance) {
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
	

	





	////追跡
	//if (condition == EnemyCondition::Tracking) {
	//
	//	//離れたらMoveへ
	//	if (distance > TRACKING_START_DISTANCE_) {
	//		condition = EnemyCondition::Move;
	//		enemy1->SetCondition(condition);
	//
	//	}
	//	
	//
	//
	//	//設定した値より短くなったら攻撃開始
	//	if (distance <= ATTACK_START_DISTANCE_ &&
	//		MINIMUM_DISTANCE < distance) {
	//	
	//		condition = EnemyCondition::Attack;
	//		enemy1->SetCondition(condition);
	//	
	//	}
	//
	//
	//
	//}



	material_.Update();

#ifdef _DEBUG
	
	ImGui::Begin("Enemy1");
	ImGui::SliderFloat("Z", &z, -10.0f, 10.0f);
	ImGui::SliderFloat("Z2", &z2, -10.0f, 10.0f);

	ImGui::End();



#endif // _DEBUG

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



