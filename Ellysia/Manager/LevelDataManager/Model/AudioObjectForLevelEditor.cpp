#include "AudioObjectForLevelEditor.h"

void AudioObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Vector3& position){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight){
	model_->Draw(worldTransform_, camera, material, directionalLight);
}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight){
	model_->Draw(worldTransform_, camera, material, pointLight);
}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material, spotLight);
}
