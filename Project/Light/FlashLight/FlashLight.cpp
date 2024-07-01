#include "FlashLight.h"
#include <VectorCalculation.h>
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
	//片方の角度
	degree_ = 15.0f;

	theta_ = 0.0f;
	phi_ = 0.0f;

	fan_.length = spotLight_.distance_;
	fan_.position = { .x = lightPosition.x,.y = lightPosition.y };
	fan_.range = 0.98f;
	fan_.directionRadian = 0.98f;


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
	lightPosition = Add(playerPosition_, { 0.0f, LIGHT_HEIGHT,0.0f });

	//計算したものをSpotLightの方に入れる
	spotLight_.position_ = lightPosition;
	spotLight_.direction_ = lightDirection_;

	//片方の角度
	float toRadian = degree_ * (std::numbers::pi_v<float> / 180.0f);
	spotLight_.cosAngle_ = std::cosf(toRadian);
	//spotLight_.cosAngle_ = 0.98f;
	
	//扇
	fan_.devidDirection = { .x=lightDirection_.x,.y= lightDirection_.z };
	fan_.length = spotLight_.distance_;
	fan_.position = { .x = lightPosition.x,.y = lightPosition.z };


#ifdef _DEBUG
	ImGui::Begin("LightFan");
	ImGui::InputFloat("Length", &fan_.length);
	ImGui::InputFloat3("Position", &fan_.position.x);
	ImGui::InputFloat("Range", &fan_.directionRadian);
	ImGui::End();

	ImGui::Begin("Light");
	ImGui::SliderFloat("Degree", &degree_,0.0f,90.0f);
	ImGui::InputFloat3("Position", &spotLight_.position_.x);
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
	spotLight_.Update();
}
