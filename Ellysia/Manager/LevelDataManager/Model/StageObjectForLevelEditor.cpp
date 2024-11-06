#include "StageObjectForLevelEditor.h"

void StageObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Transform& transform) {
	
	//レベルエディタ用のオブジェクトのタイプ
	objectType_ = LevelEditorObjectType::StageObject;

	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale = transform.scale;
	worldTransform_.rotate = transform.rotate;
	worldTransform_.translate = transform.translate;

}

void StageObjectForLevelEditor::Update(){

	//ワールドトランスフォームの更新
	worldTransform_.Update();

}

void StageObjectForLevelEditor::Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight) {

	model_->Draw(worldTransform_, camera, material, directionalLight);
}

void StageObjectForLevelEditor::Draw(const Camera& camera,const Material& material,const PointLight& pointLight) {
	model_->Draw(worldTransform_, camera, material, pointLight);
}

void StageObjectForLevelEditor::Draw(const Camera& camera,const Material& material,const SpotLight& spotLight) {
	model_->Draw(worldTransform_, camera, material, spotLight);
}

Vector3 StageObjectForLevelEditor::GetWorldPosition(){
	return worldTransform_.GetWorldPosition();
}
