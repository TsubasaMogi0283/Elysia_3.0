#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

enum TextureEffectType {
	NoneEffect,
	Monochrome,	//白黒(グレースケール)
	Sepia,		//セピア
};


class BackText{
public:
	BackText() {};

	void Initialize();


	void PreDraw();

	void Draw();

	void PostDraw();

	~BackText() {};

private:
	ComPtr<ID3D12Resource> vertexResouce_ = nullptr;
	ComPtr<ID3D12Resource> effectResource_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	//Positionだけでいい
	VertexData* vertexData_ = {};
	int32_t effectType_ = Sepia;
	int32_t* effectTypeData_ = nullptr;

	ComPtr<ID3D12Resource> centerResource_ = nullptr;
	Vector3 centerPosition_ = {};
	Vector3* positiondata_ = nullptr;

	uint32_t textureHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
};

