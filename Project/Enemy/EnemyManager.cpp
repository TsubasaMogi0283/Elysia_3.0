#include "EnemyManager.h"
#include <Input.h>
#include <cassert>

#include "Player/Player.h"
#include <VectorCalculation.h>

void EnemyManager::Initialize(uint32_t modelhandle){
	
	//Playerが空だったら引っかかるようにしている
	assert(player_!=nullptr);
	modelHandle_ = modelhandle;


	//TLのレベルエディターでやってもいいかも！
	Enemy* enemy = new Enemy();
	Vector3 position1 = {- 5.0f,0.0f,15.0f };
	enemy->Initialize(modelHandle_, position1, { 0.01f,0.0f,0.01f });
	enemy->SetRadius_(player_->GetRadius());
	enemyes_.push_back(enemy);


	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { 0.0f,0.0f,20.0f };
	enemy2->Initialize(modelHandle_, position2, { 0.0f,0.0f,0.0f });
	enemy2->SetRadius_(player_->GetRadius());
	enemyes_.push_back(enemy2);

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


	Vector3 vectorA = { 3.0f,0.0f,0.0f };
	Vector3 vectorB = { 3.0f,2.0f,0.0f };

	Vector3 project = VectorCalculation::Project(vectorB, vectorA);
	
	ImGui::Begin("ProjectTest");
	ImGui::InputFloat3("A", &vectorA.x);
	ImGui::InputFloat3("B", &vectorB.x);
	ImGui::InputFloat3("Project", &project.x);
	ImGui::End();


	//更新
	for (Enemy* enemy : enemyes_) {
		//プレイヤーとの距離を求める
		Vector3 difference = VectorCalculation::Subtract(playerPosition, enemy->GetWorldPosition());
		float distance = sqrtf(std::powf(difference.x, 2.0f) + std::powf(difference.y, 2.0f) + std::powf(difference.z, 2.0f));
		const float ATTACK_DISTANCE_OFFSET = 0.0f;
		
		float MINIMUM_DISTANCE = player_->GetRadius() + enemy->GetRadius() + ATTACK_DISTANCE_OFFSET;
		

		//現在の状態を取得
		uint32_t condition = enemy->GetCondition();




		//移動
		if (condition == EnemyCondition::Move) {
			
			//設定した値より短くなったら接近開始
			if (distance <= TRACKING_START_DISTANCE_) {
				condition = EnemyCondition::PreTracking;
				enemy->SetCondition(condition);
			}
		}


		


		//追跡
		if (condition== EnemyCondition::Tracking) {

			//離れたらMoveへ
			if (distance > TRACKING_START_DISTANCE_) {
				condition = EnemyCondition::Move;
				enemy->SetCondition(condition);

			}
			
			//設定した値より短くなったら攻撃開始
			if (distance <= ATTACK_START_DISTANCE_ &&
				MINIMUM_DISTANCE < distance) {
			
				condition = EnemyCondition::Attack;
				enemy->SetCondition(condition);

			}



			//進行方向に同じ敵がいたら止まる
			if (false) {
				condition = EnemyCondition::Attack;
				enemy->SetCondition(condition);

			}


		}

		//攻撃
		if (condition == EnemyCondition::Attack) {
			////攻撃し終わった後距離が離れていれば通常の動きに戻る
			//離れていなければもう一回攻撃
			if ((distance>=ATTACK_START_DISTANCE_&&
					distance<TRACKING_START_DISTANCE_)) {
				
				condition = EnemyCondition::Move;
				enemy->SetCondition(condition);
			}


		}



		enemy->SetPlayerPosition(playerPosition);
		enemy->Update();
		
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


