#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

class Dissolve {
public:
	Dissolve() {};

	void Initialize(uint32_t maskTexture);


	void PreDraw();

	void Draw();

	void PreDrawSecond();

	~Dissolve() {};

private:

	uint32_t srvHandle_ = 0;
	uint32_t maskTextureHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier = {};
};

