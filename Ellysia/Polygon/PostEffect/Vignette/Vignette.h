#pragma once
#include "DirectXSetup.h"
#include <Vector3.h>

class Vignette {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vignette() = default;


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
	~Vignette() = default;

public:

	/// <summary>
	/// 何倍にするかの設定
	/// </summary>
	/// <param name="scale"></param>
	inline void SetScale(const float& scale) {
		this->scale_ = scale;
	}

	/// <summary>
	/// 何乗するかの設定
	/// </summary>
	/// <param name="pow"></param>
	inline void SetPow(const float& pow) {
		this->pow_ = pow;
	}

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	inline void SetColor(const Vector3& color) {
		this->color_ = color;
	}

private:
	//送るデータ
	struct VignetteData {
		//倍
		float scale;
		//乗
		float pow;
		//色
		Vector3 color;
		float padding[3];

	};



private:

	uint32_t rtvHandle_ = 0;
	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	//SRV
	uint32_t srvHandle_ = 0;

	//
	ComPtr<ID3D12Resource> valueResource_ = nullptr;
	VignetteData* vignetteData_ = nullptr;
	float scale_ = 0.0f;
	float pow_ = 0.0f;
	Vector3 color_ = {};

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrier = {};


};

