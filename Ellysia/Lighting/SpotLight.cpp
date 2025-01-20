#include "SpotLight.h"
#include <numbers>

void SpotLight::Initialize(){
	//Resource作成
	bufferResource_ = Ellysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(SpotLightData)).Get();

	//初期値
	//色
	color_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };
	//座標
	position_ = {.x= 2.0f,.y= 1.25f,.z= 0.0f };
	//輝度
	intensity_ = 4.0f;
	//方向
	direction_={.x= -1.0f,.y= 1.0f,.z= 0.0f };
	//届く最大距離
	distance_ = 7.0f;
	//減衰率
	decay_ = 2.0f;
	//Fallowoffを制御する
	cosFallowoffStart_ = 0.3f;
	//余弦
	cosAngle_ = std::cos(std::numbers::pi_v<float> / 3.0f);
	//ライトに当たっていないところの明るさ
	aroundOffset_ = 0.0f;

}

void SpotLight::Update(){
	//書き込み
	bufferResource_->Map(0u, nullptr, reinterpret_cast<void**>(&spotLightData_));
	//色
	spotLightData_->color = color_;
	//座標
	spotLightData_->position = position_;
	//輝度
	spotLightData_->intensity = intensity_;
	//方向
	spotLightData_->direction = direction_;
	//届く最大距離
	spotLightData_->distance = distance_;
	//減衰率
	spotLightData_->decay = decay_;
	//Fallowoffを制御する
	spotLightData_->cosFallowoffStart = cosFallowoffStart_;
	//余弦
	spotLightData_->cosAngle = cosAngle_;
	//ライトに当たっていないところの明るさ
	spotLightData_->aroundOffset = aroundOffset_;
	//書き込み終了
	bufferResource_->Unmap(0u, nullptr);

}
