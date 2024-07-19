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
	TRACKING_START_DISTANCE_;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;
	ATTACK_START_DISTANCE_;

	Vector3 playerPosition = player_->GetWorldPosition();

	//更新
	for (Enemy* enemy : enemyes_) {
		//プレイヤーとの距離を求める
		Vector3 difference = VectorCalculation::Subtract(playerPosition, enemy->GetWorldPosition());
		float distance = sqrtf(std::powf(difference.x, 2.0f) + std::powf(difference.y, 2.0f) + std::powf(difference.z, 2.0f));
		const float ATTACK_DISTANCE_OFFSET = 0.0f;
		distance;
		float MINIMUM_DISTANCE = player_->GetRadius() + enemy->GetRadius() + ATTACK_DISTANCE_OFFSET;
		MINIMUM_DISTANCE;

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


