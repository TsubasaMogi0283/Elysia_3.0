#pragma once
#include "DirectXSetup.h"
#include <DirectionalLight.h>

class CreateDirectionalLight{
public:

	//初期化
	void Initialize();

	//コマンド
	void GraphicsCommand();


#pragma region アクセッサ
	void SetColor(Vector4 color) {
		this->color_ = color;
	};
	void SetDirection(Vector3 direction) {
		this->direction_ = direction;
	}
	void SetIntensity(float intencity) {
		this->intensity_ = intencity;
	}

#pragma endregion

private:
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	Vector4 color_={ 1.0f,1.0f,1.0f,1.0f };
	Vector3 direction_ = { 0.0f,-1.0f,0.0f };
	float intensity_ = 3.0f;
};