#include "IObjectForLevelEditor.h"

void BaseObjectForLevelEditor::Draw(const Camera& camera, const DirectionalLight& directionalLight){
	//平行光源に設定
	material_.lightingKinds_ = DirectionalLighting;
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_, directionalLight);
}

void BaseObjectForLevelEditor::Draw(const Camera& camera, const PointLight& pointLight){
	//点光源に設定
	material_.lightingKinds_ = PointLighting;
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_, pointLight);
}

void BaseObjectForLevelEditor::Draw(const Camera& camera, const SpotLight& spotLight){
	//スポットライトに設定
	material_.lightingKinds_ = SpotLighting;
	//変更したのでここで更新させる
	material_.Update();
	//モデルの描画
	model_->Draw(worldTransform_, camera, material_, spotLight);
}
