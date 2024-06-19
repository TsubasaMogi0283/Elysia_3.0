#include "PointLight.h"

void PointLight::Initialize(){
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLightData)).Get();

	//初期値
	//色
	color_= { 1.0f,1.0f,1.0f,1.0f };
	//位置
	position_= { 0.0f,0.0f,0.0f };
	//輝度
	intensity_ = 4.0f;
	//ライトに届く最大距離
	radius_ = 5.0f;
	//減衰率
	decay_ = 5.0f;

}

void PointLight::Update(){
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightdata_));
	pointLightdata_->color = color_;
	pointLightdata_->position= position_;
	pointLightdata_->intensity = intensity_;
	pointLightdata_->radius = radius_;
	pointLightdata_->decay = decay_;

	bufferResource_->Unmap(0, nullptr);
}
