#include "PointLight.h"

void PointLight::Initialize(){
	//Resource作成
	resource = Ellysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLightData)).Get();

	//初期値
	//色
	color_= { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };
	//位置
	position_= {.x = 0.0f,.y = 0.0f,.z = 0.0f };
	//輝度
	intensity_ = 4.0f;
	//ライトに届く最大距離
	radius_ = 5.0f;
	//減衰率
	decay_ = 5.0f;

}

void PointLight::Update(){
	//書き込み
	resource->Map(0u, nullptr, reinterpret_cast<void**>(&pointLightdata_));
	//色
	pointLightdata_->color = color_;
	//座標
	pointLightdata_->position= position_;
	//輝度
	pointLightdata_->intensity = intensity_;
	//ライトに届く最大距離
	pointLightdata_->radius = radius_;
	//減衰率
	pointLightdata_->decay = decay_;
	//書き込み終了
	resource->Unmap(0u, nullptr);
}
