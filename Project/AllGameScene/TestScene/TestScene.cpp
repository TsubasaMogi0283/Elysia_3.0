#include "TestScene.h"

#include <numbers>

#include "Input.h"
#include "ModelManager.h"
#include "LevelDataManager.h"
#include "VectorCalculation.h"
#include "CollisionCalculation.h"

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
	playerCenterPosition_ = { .x = 0.0f,.y = 0.0f,.z = -5.0f };
	playerWorldTransform_.translate = playerCenterPosition_;


	//球モデル
	uint32_t sphereModelHandle = modelManager_->LoadModelFile("Resources/Model/Sample/Sphere", "Sphere.obj");
	//四隅
	for (uint32_t i = 0; i < COUNER_QUANTITY_; ++i) {
		playerCounerModel_[i].reset(Ellysia::Model::Create(sphereModelHandle));
		playerCounerWorldTransform_[i].Initialize();

	}
	


	//カメラ
	camera_.Initialize();
	camera_.rotate.x = std::numbers::pi_v<float>/2.0f;
	camera_.translate.z = 0.0f;
	camera_.translate.y = 50.0f;
	//マテリアルの初期化
	playerMaterial_.Initialize();
	playerMaterial_.lightingKinds_ = LightingType::NoneLighting;
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

	const float SPEED = 0.1f;
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

	//座標の加算
	playerCenterPosition_= VectorCalculation::Add(playerCenterPosition_, direction);

	//プレイヤーのAABBを計算
	
	playerAABB_.max = VectorCalculation::Add(playerCenterPosition_, CUBE_SIZE);
	playerAABB_.min = VectorCalculation::Subtract(playerCenterPosition_, CUBE_SIZE);
	
	for (int i = 0; i < objects.size(); ++i) {
		FixPosition(playerAABB_, objects[i]);
	}


	//四隅の球の更新
	for (uint32_t i = 0; i < COUNER_QUANTITY_; ++i) {
		playerCounerWorldTransform_[i].Update();

	}

	//プレイヤー更新
	playerWorldTransform_.translate = playerCenterPosition_;
	playerWorldTransform_.Update();
	camera_.Update();
	directionalLight_.Update();
	playerMaterial_.Update();

	gameManager;

#ifdef _DEBUG

	ImGui::Begin("仮プレイヤー");
	ImGui::InputFloat3("中心座標", &playerCenterPosition_.x);
	ImGui::End();


	ImGui::Begin("衝突");
	ImGui::Checkbox("当たったかどうか", &isCollision_);
	ImGui::End();

	ImGui::Begin("テストシーンカメラ");
	ImGui::SliderFloat3("回転", &camera_.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("座標", &camera_.translate.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG


}

void TestScene::DrawObject3D(){
	//レベルエディタ  
	levelDataManager_->Draw(levelHandle_, camera_);
	//仮プレイヤー
	playerModel_->Draw(playerWorldTransform_,camera_, playerMaterial_);

	//四隅の球の更新
	for (uint32_t i = 0; i < COUNER_QUANTITY_; ++i) {
		playerCounerModel_[i]->Draw(playerCounerWorldTransform_[i], camera_, playerMaterial_);
	}

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


Vector3 TestScene::PushBackProcess(const AABB& aabb1, const AABB& aabb2){
	float dx1 = aabb2.max.x - aabb1.min.x; // Aを左に押す距離
	float dx2 = aabb1.max.x - aabb2.min.x; // Aを右に押す距離
	float dy1 = aabb2.max.y - aabb1.min.y; // Aを下に押す距離
	float dy2 = aabb1.max.y - aabb2.min.y; // Aを上に押す距離
	float dz1 = aabb2.max.z - aabb1.min.z; // Aを手前に押す距離
	float dz2 = aabb1.max.z - aabb2.min.z; // Aを奥に押す距離

	// 最小の押し戻し量を求める
	float minPushX = (dx1 < dx2) ? dx1 : -dx2;
	float minPushY = (dy1 < dy2) ? dy1 : -dy2;
	float minPushZ = (dz1 < dz2) ? dz1 : -dz2;

	// 一番押し戻し量が少ない軸で処理する
	if (std::abs(minPushX) <= std::abs(minPushY) && std::abs(minPushX) <= std::abs(minPushZ)) {
		return { minPushX, 0.0f, 0.0f };
	}
	else if (std::abs(minPushY) <= std::abs(minPushX) && std::abs(minPushY) <= std::abs(minPushZ)) {
		return { 0.0f, minPushY, 0.0f };
	}
	else {
		return { 0.0f, 0.0f, minPushZ };
	}
}

void TestScene::FixPosition(AABB& mainAABB, const AABB& targetAABB){
	if (CollisionCalculation::IsCollisionAABBPair(mainAABB,targetAABB)) {
		isCollision_ = true;


		Vector3 pushBack = PushBackProcess(mainAABB, targetAABB);

		// プレイヤーの中心座標も更新
		playerCenterPosition_.x += pushBack.x;
		playerCenterPosition_.y += pushBack.y;
		playerCenterPosition_.z += pushBack.z;

		mainAABB.max = VectorCalculation::Add(playerCenterPosition_, { SIZE, SIZE, SIZE });
		mainAABB.min = VectorCalculation::Subtract(playerCenterPosition_, { SIZE, SIZE, SIZE });
	}
	else {
		isCollision_ = false;
	}
}
