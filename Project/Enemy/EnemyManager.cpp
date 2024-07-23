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
	//enemyes_.push_back(enemy);


	enemy2 = new Enemy();
	Vector3 position2 = { 5.0f,0.0f,1.0f };
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
	
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;
	
	//プレイヤーの座標
	Vector3 playerPosition = player_->GetWorldPosition();
	const float ATTACK_DISTANCE_OFFSET = 0.0f;
	float MINIMUM_DISTANCE = player_->GetRadius() + enemy1->GetRadius() + ATTACK_DISTANCE_OFFSET;
	
	Vector3 difference = VectorCalculation::Subtract(playerPosition, enemy1->GetWorldPosition());
	float distance = sqrtf(std::powf(difference.x, 2.0f) + std::powf(difference.y, 2.0f) + std::powf(difference.z, 2.0f));
	
	
	uint32_t condition = enemy1->GetCondition();
	
	//移動
	if (condition == EnemyCondition::Move) {
		
		//設定した値より短くなったら接近開始
		if (distance <= TRACKING_START_DISTANCE_) {
			condition = EnemyCondition::PreTracking;
			enemy1->SetCondition(condition);

		}
	}
	
	
	
	
	
	






	enemy1->SetPlayerPosition(playerPosition);
	enemy2->SetPlayerPosition(playerPosition);

	material_.Update();

	enemy1->Update();
	enemy2->Update();



	//差分ベクトル
	Vector3 enemy1Position = enemy1->GetWorldPosition();
	Vector3 enemy2Position = enemy2->GetWorldPosition();
	float enemyRadius1 = enemy1->GetRadius();
	float enemyRadius2 = enemy2->GetRadius();

	

	Vector3 direction = enemy1->GetDirection();
	Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy2Position, enemy1Position);
	float enemyAndEnemyDistance= sqrtf(std::powf(enemyAndEnemyDifference.x, 2.0f) + std::powf(enemyAndEnemyDifference.y, 2.0f) + std::powf(enemyAndEnemyDifference.z, 2.0f));

	//正射影ベクトルを求める
	Vector3 projectVector = VectorCalculation::Project(enemyAndEnemyDifference, direction);
	

	Vector3 differenceEnemyAndProject = VectorCalculation::Subtract(enemyAndEnemyDifference, projectVector);
	float projectDistance= sqrtf(std::powf(differenceEnemyAndProject.x, 2.0f) + std::powf(differenceEnemyAndProject.y, 2.0f) + std::powf(differenceEnemyAndProject.z, 2.0f));


	
	if (enemyAndEnemyDistance < enemyRadius1 + enemyRadius2) {
#ifdef _DEBUG
		ImGui::Begin("Touch");
		ImGui::End();
#endif // _DEBUG

		if (enemyRadius1 + enemyRadius2 > projectDistance) {
#ifdef _DEBUG
			ImGui::Begin("CompleteTouch");
			ImGui::End();
#endif // _DEBUG


			uint32_t newCondition = EnemyCondition::NoneMove;
			enemy1->SetCondition(newCondition);

		}



		
	}

	





	//追跡
	if (condition == EnemyCondition::Tracking) {

		//離れたらMoveへ
		if (distance > TRACKING_START_DISTANCE_) {
			condition = EnemyCondition::Move;
			enemy1->SetCondition(condition);

		}
		


		//設定した値より短くなったら攻撃開始
		if (distance <= ATTACK_START_DISTANCE_ &&
			MINIMUM_DISTANCE < distance) {

			condition = EnemyCondition::Attack;
			enemy1->SetCondition(condition);

		}



	}

	//攻撃
	if (condition == EnemyCondition::Attack) {
		////攻撃し終わった後距離が離れていれば通常の動きに戻る
		//離れていなければもう一回攻撃
		if ((distance >= ATTACK_START_DISTANCE_ &&
			distance < TRACKING_START_DISTANCE_)) {

			condition = EnemyCondition::Move;
			enemy1->SetCondition(condition);
		}


	}

#ifdef _DEBUG
	


	ImGui::Begin("EnemyProject");
	//ImGui::InputFloat3("Difference", &enemyAndEnemyDifference.x);
	//ImGui::InputFloat3("projectVector", &projectVector.x);
	//ImGui::InputFloat3("differenceEnemyAndProject", &differenceEnemyAndProject.x);
	//ImGui::InputFloat3("1Direction", &direction.x);
	//ImGui::InputFloat("H", &projectDistance);
	//ImGui::InputFloat("Distance", &enemyAndEnemyDistance);

	ImGui::End();


#endif // _DEBUG

#ifdef _DEBUG
	//debugModelWorldTransform_.translate_ = projectVector;
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



