#include "StageObjectForLevelEditor.h"

void StageObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Vector3& position) {
	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
}

void StageObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight) {
	model_->Draw(worldTransform_, camera, material, directionalLight);
}

void StageObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight) {
	model_->Draw(worldTransform_, camera, material, pointLight);
}

void StageObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight) {
	model_->Draw(worldTransform_, camera, material, spotLight);
}
