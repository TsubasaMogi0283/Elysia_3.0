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

	//インスタンスの取得
	audio_ = Audio::GetInstance();

	//オーディオの種類によって数値を変える
	if (audioDataForLevelEditor_.type == "BGM") {
		audioType_ = AudioObjectType::BGMType;
	}
	else if (audioDataForLevelEditor_.type == "Action") {
		audioType_ = AudioObjectType::ActionType;
	}


}

void AudioObjectForLevelEditor::BackGround(){
	//接触時に鳴らす
	if (isTouch_ == true) {
		audio_->Play(audioDataForLevelEditor_.handle, audioDataForLevelEditor_.isLoop);

	}
	else {
		audio_->Stop(audioDataForLevelEditor_.handle);
	}


}

void AudioObjectForLevelEditor::Action(){

	//接触かつ動いている時に鳴らす
	if (isTouch_ == true && isListenerMove_==true) {
		audio_->Play(audioDataForLevelEditor_.handle, audioDataForLevelEditor_.isLoop);
	}
	else {
		audio_->Stop(audioDataForLevelEditor_.handle);
	}
}



void AudioObjectForLevelEditor::Update(){
	//ワールドトランスフォームの更新
	worldTransform_.Update();


	switch (audioType_) {
	case AudioObjectType::BGMType:
		//BGM用の処理
		BackGround();

		break;

	case AudioObjectType::ActionType:
		//アクションSE用
		Action();

		break;
	}
	

	
	


#ifdef _DEBUG

	Vector3 position = worldTransform_.GetWorldPosition();
	ImGui::Begin("オーディオオブジェクト"); 
	ImGui::InputFloat3("位置", &position.x);
	ImGui::Checkbox("接触", &isTouch_);
	ImGui::Checkbox("リスナーが動いているかどうか", &isListenerMove_);
	ImGui::End();
#endif // _DEBUG



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

