#include "LevelEditorSample.h"
#include <imgui.h>
#include <numbers>

#include "Input.h"
#include "GameScene/GameScene.h"
#include "AdjustmentItems.h"
#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include <VectorCalculation.h>


LevelEditorSample::LevelEditorSample(){
	//レベルエディタのインスタンスを取得
	levelEditor_ = LevelDataManager::GetInstance();
	//インプットのインスタンスを取得
	input_ = Input::GetInstance();
}

void LevelEditorSample::Initialize(){

	//平行光源の初期化
	directionalLight_.Initialize();
	
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Directional;

	//カメラ
	camera_.Initialize();
	camera_.translate_ = {.x = 0.0f,.y = 2.0f,.z = -30.0f };

	//ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();


	player_ = std::make_unique<AudioTestPlayer>();
	player_->Initialize();


	//読み込み
	levelHandle_ = levelEditor_->Load("Test/AudioAreaTestGroundAndGrass.json");

	
	//audio_->PlayMP3(audioHandleMP3_, true);



	collisionManager_ = std::make_unique<CollisionManager>();
}

void LevelEditorSample::Update(GameManager* gameManager){

	collisionManager_->ClearList();

#ifdef _DEBUG
	ImGui::Begin("Camera"); 
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x , -3.0f, 3.0f);
	ImGui::End();

	ImGui::Begin("LevelEditor");
	ImGui::Text("Rキーで指定した再読み込みするよ");
	ImGui::End();

	if (Input::GetInstance()->IsTriggerKey(DIK_R) == true) {
		levelEditor_->Reload(levelHandle_);

	}
	

#endif // _DEBUG

	gameManager;





	//移動
	playerDirection_ = {};
	//右
	if (input_->IsPushKey(DIK_D) == true) {
		playerDirection_.x += 1.0f;
	}
	//左
	else if (input_->IsPushKey(DIK_A) == true) {
		playerDirection_.x -= 1.0f;
	}
	//上
	else if (input_->IsPushKey(DIK_W) == true) {
		playerDirection_.z += 1.0f;
	}
	//下
	else if (input_->IsPushKey(DIK_S) == true) {
		playerDirection_.z -= 1.0f;
	}


	



	//プレイヤーの更新
	player_->SetDirection(playerDirection_);
	player_->Update();
	//プレイヤーのコライダーを登録
	collisionManager_->RegisterList(player_->GetCollosion());

	//レベルエディタで使うリスナーの設定
	Listener listener = {
		.position = player_->GetWorldPosition(),
		.move = player_->GetDirection(),
	};
	levelEditor_->SetListener(levelHandle_, listener);

	//レベルエディタの更新
	levelEditor_->Update(levelHandle_);
	//マテリアルの更新
	material_.Update();
	//平行光源の更新
	directionalLight_.Update();


	//レベルエディタにあるコライダーを登録
	std::vector<IObjectForLevelEditorCollider*> g = levelEditor_->GetCollider(levelHandle_);
	for (auto it = g.begin(); it != g.end(); ++it) {
		collisionManager_->RegisterList(*it);
	}


	//衝突判定の計算
	collisionManager_->CheckAllCollision();




	//カメラの更新
	//高さの補正も足す
	const Vector3 OFFSET = { .x = 0.0f,.y = 2.0f,.z = 0.0f };
	Vector3 playerViewPoint = VectorCalculation::Add(player_->GetWorldPosition(), OFFSET);
	camera_.translate_ = playerViewPoint;
	camera_.Update();

}

void LevelEditorSample::DrawSpriteBack()
{
}

void LevelEditorSample::DrawObject3D(){
	//レベルエディタのモデルを描画     
	levelEditor_->Draw(levelHandle_,camera_, material_, directionalLight_);
}

void LevelEditorSample::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void LevelEditorSample::DrawPostEffect(){
	back_->Draw();
}

void LevelEditorSample::DrawSprite(){


}

LevelEditorSample::~LevelEditorSample(){
}
