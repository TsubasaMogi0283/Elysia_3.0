#include "Gate.h"

void Gate::Initialize(uint32_t modelHandle){
	//モデル
	model_ = std::make_unique<Model>();
	model_.reset(Model::Create(modelHandle));

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Spot;

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.scale.x = 8.0f;
	worldTransform_.scale.y = 5.0f;

	worldTransform_.translate.z = 40.0f;

	width_ = worldTransform_.scale.x;
	distance_ = 10.0f;

}

void Gate::Update(){


#ifdef _DEBUG
	ImGui::Begin("Gate");
	ImGui::InputFloat3("Translate", &worldTransform_.translate.x);
	ImGui::End();
#endif


	//更新
	worldTransform_.Update();
	material_.Update();
}

void Gate::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}

Vector3 Gate::GetWorldPosition() const{
	Vector3 result = {};
	result.x = worldTransform_.worldMatrix.m[3][0];
	result.y = worldTransform_.worldMatrix.m[3][1];
	result.z = worldTransform_.worldMatrix.m[3][2];

	return result;
}

bool Gate::isCollision(Vector3 playerPosition){



	
	Vector3 gateCenterPosition = GetWorldPosition();
	
	
	//
	float leftPosition = gateCenterPosition.x - width_;
	float rightPosition = gateCenterPosition.x + width_;
	float frontPosition = gateCenterPosition.z - distance_;


	//範囲内にいる時
	if (playerPosition.x >= leftPosition &&
		playerPosition.x <= rightPosition&&
		playerPosition.z >= frontPosition &&
		playerPosition.z <= gateCenterPosition.z){
		return true;
	}

	else {
		return false;
	}


}
