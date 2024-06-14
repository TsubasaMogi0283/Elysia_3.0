#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


class RadialBlur {
public:
	RadialBlur() {};

	void Initialize();


	void PreDraw();

	void Draw();

	void PostDraw();

	~RadialBlur() {};

private:
	uint32_t textureHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};

