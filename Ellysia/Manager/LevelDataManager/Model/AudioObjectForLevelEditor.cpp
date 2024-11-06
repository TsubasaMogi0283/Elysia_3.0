#include "AudioObjectForLevelEditor.h"



void AudioObjectForLevelEditor::Initialize(const uint32_t& modelhandle, const Transform& transform){
	
	//レベルエディタ用のオブジェクトのタイプ
	objectType_ = LevelEditorObjectType::AudioObject;
	
	//モデルの生成
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale = transform.scale;
	worldTransform_.rotate = transform.rotate;
	worldTransform_.translate = transform.translate;


	audio_ = Audio::GetInstance();


}

void AudioObjectForLevelEditor::Update(){
	//ワールドトランスフォームの更新
	worldTransform_.Update();


}


void AudioObjectForLevelEditor::Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight){
	//モデルの描画
	model_->Draw(worldTransform_, camera, material, directionalLight);
}

void AudioObjectForLevelEditor::Draw(const Camera& camera,const Material& material,const PointLight& pointLight){
	//モデルの描画
	model_->Draw(worldTransform_, camera, material, pointLight);
}

void AudioObjectForLevelEditor::Draw(const Camera& camera,const Material& material,const SpotLight& spotLight){
	//モデルの描画
	model_->Draw(worldTransform_, camera, material, spotLight);
}

Vector3 AudioObjectForLevelEditor::GetWorldPosition(){
	return worldTransform_.GetWorldPosition();
}
