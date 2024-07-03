#include "FlashLight.h"
#include <VectorCalculation.h>
#include "Camera.h"
#include <numbers>

void FlashLight::Initialize(){



	//初期化
	spotLight_.Initialize();

	spotLight_.position_ = lightPosition;
	spotLight_.distance_ = 35.0f;
	spotLight_.decay_ = 0.6f;
	spotLight_.cosFallowoffStart_ = 6.1f;
	//spotLight_.cosAngle_ = 0.98f;
	spotLight_.intensity_ = 200.0f;
	//ライトの片方の角度
	//15度=π/12
	lightSideTheta = (std::numbers::pi_v<float>/12.0f);

	theta_ = 0.0f;
	phi_ = 0.0f;

	fan_.length = spotLight_.distance_;
	fan_.position = { .x = lightPosition.x,.y = lightPosition.y };
	fan_.range = 0.98f;
	fan_.directionRadian = 0.98f;

	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	

	const float SCALE = 0.4f;
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i].reset(Model::Create(modelHandle));
		worldTransform_[i].Initialize();
		worldTransform_[i].scale_ = { .x = SCALE,.y = SCALE ,.z = SCALE };
		
	}

	material_.Initialize();
	material_.lightingKinds_ = None;
	material_.color_ = { 0.5f,1.0f,0.5f,1.0f };

	lightCenterModel_.reset(Model::Create(modelHandle));
	lightCenterWorldTransform_.Initialize();
	lightCenterWorldTransform_.scale_ = { .x = SCALE,.y = SCALE ,.z = SCALE };
	lightCnterMaterial_.Initialize();
	lightCnterMaterial_.lightingKinds_ = None;


}

void FlashLight::Update(){
	

	//ライトの方向ベクトル
	Vector3 direction = {};
	direction.x = std::cosf(theta_);
	direction.z = std::sinf(theta_);

	//Yを求めたい
	Vector3 directionForY = {};
	//上のdirectionから長さを求めてからatan2でyを出す
	float lengthXZ = sqrtf(std::powf(direction.x, 2.0f) + std::powf(direction.z, 2.0f));
	lightDirection_.x = direction.x;
	lightDirection_.y = lengthXZ * std::tanf(phi_);
	lightDirection_.z = direction.z;


	//プレイヤーの座標と微調整分
	const float LIGHT_HEIGHT = 1.0f;
	const Vector3 OFFSET = { 0.0f, LIGHT_HEIGHT,0.0f };
	lightPosition = Add(playerPosition_, OFFSET);

	//計算したものをSpotLightの方に入れる
	spotLight_.position_ = lightPosition;
	spotLight_.direction_ = lightDirection_;

	//片方の角度
	spotLight_.cosAngle_ = std::cosf(lightSideTheta);
	
	//扇
	fan_.centerRadian = theta_;
	fan_.leftSideRadian = theta_ + lightSideTheta;
	fan_.rightSideRadian = theta_ - lightSideTheta;

	fan_.divideDirection = { .x=lightDirection_.x,.y= lightDirection_.z };
	fan_.length = spotLight_.distance_;
	fan_.position = { .x = lightPosition.x,.y = lightPosition.z };


	//端をデバッグ用として可視化
	const float DISTANCE = 20.0f;
	Vector2 fanLeft = { .x = std::cosf(theta_ + lightSideTheta)* DISTANCE,.y = std::sinf(theta_ + lightSideTheta)* DISTANCE };
	Vector2 fanRight = { .x = std::cosf(theta_ - lightSideTheta)* DISTANCE,.y = std::sinf(theta_ - lightSideTheta)* DISTANCE };

	fan_.leftVector = { .x = std::cosf(theta_ + lightSideTheta),.y = std::sinf(theta_ + lightSideTheta) };
	fan_.rightVector = { .x = std::cosf(theta_ - lightSideTheta),.y = std::sinf(theta_ - lightSideTheta) };

	worldTransform_[Left].translate_ = Add(playerPosition_,{ fanLeft.x ,0.0f,fanLeft.y });
	worldTransform_[Right].translate_ = Add(playerPosition_,{ fanRight.x ,0.0f,fanRight.y });



	//中心
	lightCenterWorldTransform_.translate_ = lightPosition;


#ifdef _DEBUG
	ImGui::Begin("LightFan");
	ImGui::InputFloat("Length", &fan_.length);
	ImGui::InputFloat3("Position", &fan_.position.x);
	ImGui::InputFloat("Range", &fan_.directionRadian);
	ImGui::InputFloat("LeftSideRadian", &fan_.leftSideRadian);
	ImGui::InputFloat("CnterRadian", &fan_.centerRadian);
	ImGui::InputFloat("RightSideRadian", &fan_.rightSideRadian);



	ImGui::InputFloat2("DevidDirection", &fan_.divideDirection.x);
	ImGui::InputFloat2("FanLeft", &fanLeft.x);
	ImGui::InputFloat2("FanRight", &fanRight.x);


	ImGui::End();

	ImGui::Begin("Light");
	ImGui::SliderFloat("Degree", &lightSideTheta,0.0f,90.0f);
	ImGui::InputFloat3("Position", &spotLight_.position_.x);
	ImGui::InputFloat("lengthXZ",&lengthXZ);
	ImGui::InputFloat3("Direction", &spotLight_.direction_.x);
	ImGui::SliderFloat("Distance", &spotLight_.distance_, 0.0f, 100.0f);
	ImGui::SliderFloat("Decay", &spotLight_.decay_, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", &spotLight_.cosFallowoffStart_, 0.0f, 20.0f);
	ImGui::SliderFloat("CosAngle", &spotLight_.cosAngle_, 0.0f, 3.0f);
	ImGui::SliderFloat("intencity_", &spotLight_.intensity_, 0.0f, 400.0f);
	ImGui::InputFloat("Theta", &theta_);
	ImGui::InputFloat("Phi", &phi_);
	ImGui::End();
#endif // _DEBUG

	

	//更新
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		worldTransform_[i].Update();
	}
	lightCenterWorldTransform_.Update();
	lightCnterMaterial_.Update();
	material_.Update();
	spotLight_.Update();
}

void FlashLight::Draw(Camera& camera){
	//端
	for (uint32_t i = 0; i < SIDE_QUANTITY_; ++i) {
		model_[i]->Draw(worldTransform_[i], camera, material_, spotLight_);
	}
	//中心
	lightCenterModel_->Draw(lightCenterWorldTransform_,camera, lightCnterMaterial_,spotLight_);

	
}
