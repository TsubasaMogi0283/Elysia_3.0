#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


class RandomEffect{
public:
	RandomEffect() {};

	void Initialize();


	void PreDraw();

	void Draw();

	void PreDrawSecond();

	~RandomEffect() {};

private:
	struct DissolveInformation {
		float threshold;
	};
	ComPtr<ID3D12Resource> thresholdResource_ = nullptr;
	DissolveInformation dissolveInformation_ = {};
	DissolveInformation* thresholdData_ = nullptr;


	uint32_t srvHandle_ = 0;
	uint32_t maskTextureHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier = {};
};

