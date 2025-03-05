#include "TestScene.h"

#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "VectorCalculation.h"

TestScene::TestScene(){
	//インスタンスの取得	
	//入力
	input_ = Ellysia::Input::GetInstance();
	//モデル管理クラス
	modelManager_ = Ellysia::ModelManager::GetInstance();
	//レベルエディタ管理クラス
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();
}

void TestScene::Initialize(){

	//ハンドルの取得
	levelHandle_ = levelDataManager_->Load("CollisionTest/CollisionTest.json");

	//仮プレイヤー
	uint32_t playerModelHandle = modelManager_->LoadModelFile("Resources/Model/Sample/Cube", "cube.obj");
	playerModel_.reset(Ellysia::Model::Create(playerModelHandle));
	playerWorldTransform_.Initialize();

	//カメラ
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float>/2.0f;
	camera_.translate.z = 0.0f;
	camera_.translate.y = 50.0f;
	//マテリアルの初期化
	playerMaterial_.Initialize();
	playerMaterial_.lightingKinds_ = LightingType::DirectionalLighting;
	//平行光源の初期化
	directionalLight_.Initialize();

	//背景
	backTexture_ = std::make_unique<Ellysia::BackTexture>();
	backTexture_->Initialize();
}

void TestScene::Update(Ellysia::GameManager* gameManager){


	//再読み込み
	if (input_->IsTriggerKey(DIK_R) == true) {
		levelDataManager_->Reload(levelHandle_);
	}

	//レベルエディタの更新
	levelDataManager_->Update(levelHandle_);



	std::vector<AABB> objects = levelDataManager_->GetStageObjectAABBs(levelHandle_);



	for (auto i = 0; i < objects.size(); ++i) {
		//片側のサイズ
		Vector3 sideSize = VectorCalculation::Subtract(objects[i].max, objects[i].min);
		sideSize;


	}


	const float SPEED = 0.05f;
	//方向
	Vector3 direction = {};

	//入力
	if (input_->IsPushKey(DIK_UP) == true) {
		direction.z = SPEED;
	}
	else if (input_->IsPushKey(DIK_DOWN) == true) {
		direction.z = -SPEED;
	}
	else if (input_->IsPushKey(DIK_RIGHT) == true) {
		direction.x = SPEED;
	}
	else if (input_->IsPushKey(DIK_LEFT) == true) {
		direction.x = -SPEED;
	}

	playerWorldTransform_.translate = VectorCalculation::Add(playerWorldTransform_.translate, direction);






#ifdef _DEBUG
	ImGui::Begin("テストシーンカメラ");
	ImGui::SliderFloat3("回転", &camera_.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG


	//プレイヤー更新
	playerWorldTransform_.Update();
	camera_.Update();
	directionalLight_.Update();
	playerMaterial_.Update();

	gameManager;
}

void TestScene::DrawObject3D(){
	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_);
	//仮プレイヤー
	playerModel_->Draw(playerWorldTransform_,camera_, playerMaterial_);

}

void TestScene::PreDrawPostEffectFirst(){
	//描画前処理
	backTexture_->PreDraw();
}

void TestScene::DrawPostEffect(){
	//描画前処理
	backTexture_->Draw();
}

void TestScene::DrawSprite()
{
}
