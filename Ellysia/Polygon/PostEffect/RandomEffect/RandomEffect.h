#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include <time.h>
#include <random>

class RandomEffect{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RandomEffect()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理1
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RandomEffect()=default;

private:
	struct RandomValue {
		float value;
	};
	ComPtr<ID3D12Resource> randomValueResource_ = nullptr;
	RandomValue randomValue_ = {};
	RandomValue* randomValueData_ = nullptr;


	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	uint32_t rtvHandle_ = 0;



	uint32_t srvHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier = {};
	std::mt19937 randomEngine_ = {};
};

