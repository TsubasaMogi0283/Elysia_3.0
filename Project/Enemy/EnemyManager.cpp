#include "EnemyManager.h"
#include <Input.h>

void EnemyManager::Initialize(uint32_t modelhandle){
	//TLのレベルエディターでやってもいいかも！
	
	Enemy* enemy = new Enemy();
	Vector3 position1 = { -3.0f,0.0f,0.0f };
	modelHandle_ = modelhandle;
	enemy->Initialize(modelHandle_, position1, { 0.01f,0.0f,0.0f });
	enemyes_.push_back(enemy);


	Enemy* enemy2 = new Enemy();
	Vector3 position2 = { 4.0f,0.0f,0.0f };
	enemy2->Initialize(modelHandle_, position2, { 0.0f,0.0f,0.0f });
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
	enemyes_.push_back(enemy);

}

void EnemyManager::Update(){

#ifdef _DEBUG
	//Gキーで出す
	if (Input::GetInstance()->IsTriggerKey(DIK_G) == true) {
		GenarateEnemy();
	}

#endif


	//更新
	for (Enemy* enemy : enemyes_) {
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


