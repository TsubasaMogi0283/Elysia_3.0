#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include <HSV.h>

class HSVFilter{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HSVFilter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HSVFilter() = default;

private:
	//別々で分けたい
	ComPtr<ID3D12Resource> hsvResource_ = nullptr;
	HSV* hsvData_ = nullptr;
	HSV hsv_ = {};

	//RTV
	uint32_t rtvHandle_ = 0;
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	//SRV
	uint32_t srvHandle_ = 0;

	D3D12_RESOURCE_BARRIER barrier = {};



};

