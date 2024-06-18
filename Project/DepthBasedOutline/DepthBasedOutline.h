#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "Matrix4x4.h"
struct Camera;


class DepthBasedOutline{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DepthBasedOutline() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前の処理
	/// </summary>
	void PreDraw();
	void PreDrawSecond();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera& camera);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~DepthBasedOutline() = default;

private:
	struct CameraMatrix {
		Matrix4x4 projectionInverse;
	};



private:

	uint32_t textureHandle_ = 0;
	uint32_t srvforDepthHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier = {};

	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	uint32_t rtvHandle_ = 0;


	ComPtr<ID3D12Resource> projectionInverseResource_ = nullptr;
	CameraMatrix* cameraMatrix_ = nullptr;

};

