#include "AudioObjectForLevelEditor.h"

void AudioObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Vector3& position){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	worldTransform_.Initialize();

}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight){

}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight){

}

void AudioObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight){

}
