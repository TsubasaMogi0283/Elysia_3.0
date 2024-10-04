#include "SpotLight.h"
#include <numbers>

void SpotLight::Initialize(){
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(SpotLightData)).Get();

	//初期値
	//ライトの色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	//ライトの位置
	position_ = { 2.0f,1.25f,0.0f };
	//輝度
	intensity_ = 4.0f;

	//スポットライトの方向
	direction_={ -1.0f,1.0f,0.0f };
	//ライトの届く最大距離
	distance_ = 7.0f;
	//減衰率
	decay_ = 2.0f;
	//Fallowoffを制御する
	cosFallowoffStart_ = 0.3f;
	//スポットライトの余弦
	cosAngle_ = std::cos(std::numbers::pi_v<float> / 3.0f);

	aroundOffset_ = 0.0f;

}

void SpotLight::Update(){
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	spotLightData_->color = color_;
	spotLightData_->position = position_;
	spotLightData_->intensity = intensity_;
	spotLightData_->direction = direction_;
	spotLightData_->distance = distance_;
	spotLightData_->decay = decay_;
	spotLightData_->cosFallowoffStart = cosFallowoffStart_;
	spotLightData_->cosAngle = cosAngle_;
	spotLightData_->aroundOffset = aroundOffset_;

	bufferResource_->Unmap(0, nullptr);

}
