#pragma once
#include "DirectXSetup.h"
#include "VertexData.h"

class RenderTexture{
public:

	RenderTexture();



	void Initialize();


	void BeginDraw();


	void EndDraw();

	~RenderTexture();


public:


private:

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//描画
	VertexData* vertexData_ = nullptr;
	UINT backBufferIndex_;
	D3D12_RESOURCE_BARRIER barrier_{};
};

