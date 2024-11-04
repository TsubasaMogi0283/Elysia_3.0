#include "StageObjectForLevelEditor.h"

void StageObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Vector3& position) {
	
	//レベルエディタ用のオブジェクトのタイプ
	objectType_ = LevelEditorObjectType::StageObject;

	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate = position;

	//当たり判定
	stageObjectForLevelEditorCollider_ = std::make_unique<StageObjectForLevelEditorCollider>();

}

void StageObjectForLevelEditor::Update(){

	//ワールドトランスフォームの更新
	worldTransform_.Update();

	//当たり判定用に座標を設定する
	stageObjectForLevelEditorCollider_->SetObjectPosition(worldTransform_.GetWorldPosition());
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
