#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"

class BackText{
public:
	BackText() {};

	void Initialize();

	void Update();

	void Draw();

	~BackText() {};

private:
	ComPtr<ID3D12Resource> vertexResouce_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	//Positionだけでいい
	Vector4* vertexData_ = {};
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector4* materialData_ = nullptr;
};

