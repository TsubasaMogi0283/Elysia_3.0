#include "AudioTestPlayer.h"
#include "VectorCalculation.h"
#include "ModelManager.h"

void AudioTestPlayer::Initialize(){
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	//モデルの生成
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Cube","cube.obj");
	model_.reset(Model::Create(modelHandle));
	
	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = LightingType::Directional;

	//コリジョンの初期化
	collosion_ = std::make_unique<PlayerCollisionToAudioObject>();
	collosion_->Initialize();
}

void AudioTestPlayer::Update(){

#ifdef _DEBUG
	ImGui::Begin("音確認用のプレイヤー");
	ImGui::SliderFloat3("位置", &worldTransform_.translate.x,-40.0f,40.0f);
	ImGui::End();

#endif // _DEBUG
	//方向とスピードを計算
	const float SPEED = 0.1f;
	Vector3 newDirection = VectorCalculation::Multiply(direction_, SPEED);

	//位置の更新
	worldTransform_.translate = VectorCalculation::Add(worldTransform_.translate, newDirection);
	worldTransform_.Update();
	material_.Update();

	collosion_->SetPlayerGetWorldPosition(worldTransform_.GetWorldPosition());
	collosion_->Update();
}

void AudioTestPlayer::Draw(const Camera& camera, const DirectionalLight& directionalLight){
	model_->Draw(worldTransform_, camera, material_, directionalLight);

}

