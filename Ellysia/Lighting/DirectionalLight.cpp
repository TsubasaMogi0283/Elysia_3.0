#include "DirectionalLight.h"

void DirectionalLight::Initialize(){
	//Resource作成
	bufferResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLightData)).Get();

	//初期
	//ライトの色
	color = {1.0f,1.0f,1.0f,1.0f};
	//ライトの向き
	direction= { 0.0f,-1.0f,0.0f };
	//輝度
	intensity= 1.0f;


}

void DirectionalLight::Update(){
	bufferResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = color;
	directionalLightData_->direction = direction;
	directionalLightData_->intensity = intensity;
	bufferResource->Unmap(0, nullptr);
}

