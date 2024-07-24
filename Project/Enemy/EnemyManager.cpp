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
	enemy1 = new Enemy();
	Vector3 position1 = {0.0f,0.0f,9.0f };
	enemy1->Initialize(modelHandle_, position1, { 0.01f,0.0f,0.00f });
	enemy1->SetRadius_(player_->GetRadius());
	//enemyes_.push_back(enemy1);

	z = position1.z;

	enemy2 = new Enemy();
	Vector3 position2 = { 5.0f,0.0f,9.0f };
	enemy2->Initialize(modelHandle_, position2, { 0.0f,0.0f,-0.0f });
	enemy2->SetRadius_(player_->GetRadius());
	//enemyes_.push_back(enemy2);



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
	
	uint32_t condition = enemy1->GetCondition();
	
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
	
	//enemy1->SetPlayerPosition(playerPosition);
	//enemy2->SetPlayerPosition(playerPosition);
	//
	//enemy1->Update();
	//enemy2->Update();


	

	
	//enemy1->SetPositionZ(z);

	//差分ベクトル
	//Vector3 enemy1Position = enemy1->GetWorldPosition();
	//Vector3 enemy2Position = enemy2->GetWorldPosition();
	//float enemyRadius1 = enemy1->GetRadius();
	//float enemyRadius2 = enemy2->GetRadius();


	for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {
		for (std::list<Enemy*>::iterator it2 = std::next(it1); it2 != enemyes_.end(); ++it2) {
			Enemy* enemy1 = *it1;
			Enemy* enemy2 = *it2;

			//ワールド座標
			Vector3 enemy1Position = enemy1->GetWorldPosition();
			Vector3 enemy2Position = enemy2->GetWorldPosition();
			//半径
			float enemyRadius1 = enemy1->GetRadius();
			float enemyRadius2 = enemy2->GetRadius();

			//向き
			Vector3 direction = enemy1->GetDirection();
			Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
			float enemyAndEnemyDistance = sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));

			// 正射影ベクトルを求める
			Vector3 projectVector = VectorCalculation::Project(enemyAndEnemyDifference, direction);

			Vector3 differenceEnemyAndProject = VectorCalculation::Subtract(enemyAndEnemyDifference, projectVector);
			float projectDistance = sqrtf(std::powf(differenceEnemyAndProject.x, 2.0f) + std::powf(differenceEnemyAndProject.y, 2.0f) + std::powf(differenceEnemyAndProject.z, 2.0f));

			uint32_t condition = enemy1->GetCondition();

			// 進行方向上にいたら
			if ((enemyRadius1 + enemyRadius2 > projectDistance) &&
				(condition == EnemyCondition::Move || condition == EnemyCondition::Tracking)) {
				
				#ifdef _DEBUG
				ImGui::Begin("On the direction of progress");
				ImGui::End();
				#endif // _DEBUG

				// 接触していたら
				if (enemyAndEnemyDistance < enemyRadius1 + enemyRadius2) {
					
					#ifdef _DEBUG
					ImGui::Begin("Touch");
					ImGui::End();
					#endif // _DEBUG

					uint32_t newCondition = EnemyCondition::NoneMove;
					enemy1->SetCondition(newCondition);
				}
				// 接触していない
				else {

					#ifdef _DEBUG
					ImGui::Begin("NotTouch");
					ImGui::End();
					#endif // _DEBUG

					uint32_t newCondition = EnemyCondition::Move;
					enemy1->SetCondition(newCondition);
				}
			}

			if (enemyRadius1 + enemyRadius2 <= projectDistance) {

				if (condition == EnemyCondition::Move) {
					uint32_t newCondition = EnemyCondition::Move;
					enemy1->SetCondition(newCondition);
				}
				if (condition == EnemyCondition::Tracking) {
					uint32_t newCondition = EnemyCondition::Tracking;
					enemy1->SetCondition(newCondition);
				}
				
			}
		
		}
	}

	

//	Vector3 direction = enemy1->GetDirection();
//	Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
//	float enemyAndEnemyDistance= sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));
//
//	//正射影ベクトルを求める
//	Vector3 projectVector = VectorCalculation::Project(enemyAndEnemyDifference, direction);
//	Vector3 differenceEnemyAndProject = VectorCalculation::Subtract(enemyAndEnemyDifference, projectVector);
//	float projectDistance= sqrtf(std::powf(differenceEnemyAndProject.x, 2.0f) + std::powf(differenceEnemyAndProject.y, 2.0f) + std::powf(differenceEnemyAndProject.z, 2.0f));
//
//
//	Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
//	dot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);
//
//
//	//進行方向上にいたら
//	if ((enemyRadius1 + enemyRadius2 > projectDistance)&&
//		(condition== EnemyCondition::Move || condition == EnemyCondition::Tracking)) {
//
//		
//
//#ifdef _DEBUG
//		ImGui::Begin("On the direction of progress");
//		
//		ImGui::End();
//#endif // _DEBUG
//
//		
//		//接触していたら
//		if ((enemyAndEnemyDistance < enemyRadius1 + enemyRadius2)&& dot) {
//#ifdef _DEBUG
//			ImGui::Begin("Touch");
//			ImGui::End();
//#endif // _DEBUG
//			
//
//			uint32_t newCondition = EnemyCondition::NoneMove;
//			enemy1->SetCondition(newCondition);
//
//		}
//		//接触していない
//		else {
//#ifdef _DEBUG
//			ImGui::Begin("NotTouch");
//			ImGui::End();
//#endif // _DEBUG
//
//			uint32_t newCondition = EnemyCondition::Move;
//			enemy1->SetCondition(newCondition);
//
//		}
//	}
//	if (enemyRadius1 + enemyRadius2 < projectDistance) {
//		uint32_t newCondition = EnemyCondition::Move;
//		enemy1->SetCondition(newCondition);
//
//	}

	





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





	////ステージの外に行かないようにする
	////左
	//if (stageRect_.leftBack.x < enemy1->GetWorldPosition().x - enemy1->GetRadius()) {
	//	enemy1->InvertSpeedX();
	//}
	////右
	//if (stageRect_.rightBack.x > enemy1->GetWorldPosition().x + enemy1->GetRadius()) {
	//	enemy1->InvertSpeedX();
	//}
	////後ろ
	//if (stageRect_.leftBack.z < enemy1->GetWorldPosition().z + enemy1->GetRadius()) {
	//	enemy1->InvertSpeedZ();
	//}
	////前
	//if (stageRect_.leftFront.z > enemy1->GetWorldPosition().z - enemy1->GetRadius()) {
	//	enemy1->InvertSpeedZ();
	//}


	material_.Update();

#ifdef _DEBUG
	
	ImGui::Begin("Enemy1");
	ImGui::SliderFloat("Z", &z, -10.0f, 10.0f);
	ImGui::InputFloat("dot", &dot);
	ImGui::End();

	//ImGui::Begin("EnemyProject");
	//
	//ImGui::InputFloat3("Difference", &enemyAndEnemyDifference.x);
	//ImGui::InputFloat3("projectVector", &projectVector.x);
	//ImGui::InputFloat3("differenceEnemyAndProject", &differenceEnemyAndProject.x);
	//ImGui::InputFloat3("1Direction", &direction.x);
	//ImGui::InputFloat("H", &projectDistance);
	//
	//ImGui::InputFloat("Distance", &enemyAndEnemyDistance);
	//
	//ImGui::End();


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

	enemy1->Draw(camera, spotLight);
	enemy2->Draw(camera, spotLight);
}

EnemyManager::~EnemyManager(){
	for (Enemy* enemy : enemyes_) {
		delete enemy;
	}
	delete enemy1;
	delete enemy2;


}



