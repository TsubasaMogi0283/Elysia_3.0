#include "CreateDirectionalLight.h"



void CreateDirectionalLight::Initialize(){

	directionalLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLight)).Get();
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 3.0f;

	directionalLightResource_->Unmap(0, nullptr);





}


void CreateDirectionalLight::GraphicsCommand(){
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = color_;
	directionalLightData_->direction = direction_;
	directionalLightData_->intensity = intensity_;
	directionalLightResource_->Unmap(0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

}