#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


class OutLine{
public:
	OutLine() {};

	void Initialize();


	void PreDraw();

	void Draw();

	void PostDraw();

	~OutLine() {};

private:
	ComPtr<ID3D12Resource> effectResource_ = nullptr;

	int32_t effectType_ = 0;
	int32_t* effectTypeData_ = nullptr;


	uint32_t textureHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};

