#include "StageObjectForLevelEditor.h"

void StageObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Vector3& position) {
	
	//レベルエディタ用のオブジェクトのタイプ
	objectType_ = LevelEditorObjectType::StageObject;

	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;

	//当たり判定
	stageObjectForLevelEditorCollider_ = std::make_unique<StageObjectForLevelEditorCollider>();

}

void StageObjectForLevelEditor::Update(){

	//ワールドトランスフォームの更新
	worldTransform_.Update();

	//当たり判定用に座標を設定する
	stageObjectForLevelEditorCollider_->SetObjectPosition(worldTransform_.GetWorldPosition());
}

void StageObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight) {
	worldTransform;
	model_->Draw(worldTransform_, camera, material, directionalLight);
}

void StageObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight) {
	worldTransform;
	model_->Draw(worldTransform_, camera, material, pointLight);
}

void StageObjectForLevelEditor::Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight) {
	worldTransform;
	model_->Draw(worldTransform_, camera, material, spotLight);
}
