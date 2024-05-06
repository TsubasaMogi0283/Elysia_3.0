#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

class BackText{
public:
	BackText() {};

	void Initialize();

	void Update();

	void PreDraw();

	void Draw();

	void PostDraw();

	~BackText() {};

private:
	ComPtr<ID3D12Resource> vertexResouce_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	//Positionだけでいい
	VertexData* vertexData_ = {};
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector4* materialData_ = nullptr;

	ComPtr<ID3D12Resource> centerResource_ = nullptr;
	Vector3 centerPosition_ = {};
	Vector3* positiondata_ = nullptr;

	uint32_t textureHandle_ = 0;


	D3D12_RESOURCE_BARRIER barrier_{};
};

