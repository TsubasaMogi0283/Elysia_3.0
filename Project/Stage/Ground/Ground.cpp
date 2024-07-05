#include "Ground.h"

void Ground::Initialize(uint32_t modelhandle){
	model_.reset(Model::Create(modelhandle));

	//ワールドトランスフォームの食器化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { SCALE_SIZE_,SCALE_SIZE_,SCALE_SIZE_ };
	worldTransform_.translate_.x = 0.0f;
	worldTransform_.translate_.y = 0.0f;

	//四隅
	stageRect_.leftBack = { .x = -SCALE_SIZE_ ,.y = 0.0f ,.z = SCALE_SIZE_ };
	stageRect_.rightBack = { .x = SCALE_SIZE_ ,.y = 0.0f ,.z = SCALE_SIZE_ };
	stageRect_.leftFront = { .x = -SCALE_SIZE_ ,.y = 0.0f ,.z = -SCALE_SIZE_ };
	stageRect_.rightFront = { .x = SCALE_SIZE_ ,.y = 0.0f ,.z = -SCALE_SIZE_ };


	//マテリアルの消去
	material_.Initialize();
	material_.lightingKinds_ = Spot;

}

void Ground::Update(){
	//更新
	worldTransform_.Update();
	material_.Update();

}

void Ground::Draw(Camera& camera, SpotLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}
