#include "AudioObjectForLevelEditor.h"



void AudioObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Vector3& position){
	
	//レベルエディタ用のオブジェクトのタイプ
	objectType_ = LevelEditorObjectType::AudioObject;
	
	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

	//コライダーの生成
	audioObjectForLevelEditorCollider_ = std::make_unique<AudioObjectForLevelEditorCollider>();
	

}

void AudioObjectForLevelEditor::Update(){
	//ワールドトランスフォームの更新
	worldTransform_.Update();

	//コライダーに座標を送る
	audioObjectForLevelEditorCollider_->SetObjectPosition(worldTransform_.GetWorldPosition());

}

void AudioObjectForLevelEditor::SetAudio(const std::string& filePath){
	filePath;
	//オーディオの生成
	//audio_.reset(Audio::GetInstance());
	//audioHandle_=audio_->Load(filePath);

}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight){

	worldTransform;
	model_->Draw(worldTransform_, camera, material, directionalLight);
}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight){
	worldTransform;
	model_->Draw(worldTransform_, camera, material, pointLight);
}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight){
	worldTransform;
	model_->Draw(worldTransform_, camera, material, spotLight);
}
