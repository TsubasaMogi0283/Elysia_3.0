#include "DirectionalLight.h"

void DirectionalLight::Initialize(){
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLightData)).Get();

	//初期
	//ライトの色
	color_ = {1.0f,1.0f,1.0f,1.0f};
	//ライトの向き
	direction_= { 0.0f,-1.0f,0.0f };
	//輝度
	intensity_= 1.0f;


}

void DirectionalLight::Update(){
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));
	directionalLight_->color = color_;
	directionalLight_->direction = direction_;
	directionalLight_->intensity = intensity_;
	bufferResource_->Unmap(0, nullptr);
}

