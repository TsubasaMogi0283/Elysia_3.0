#pragma once
#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"

class Dissolve {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Dissolve()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="maskTexture"></param>
	void Initialize(uint32_t maskTexture);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デストラク
	/// </summary>
	~Dissolve()=default;

public:
	/// <summary>
	/// Edgeを使うかどうか
	/// </summary>
	/// <param name="useEdge"></param>
	inline void SetUseEdge(bool useEdge) {
		this->dessolveValue_.isUseEdge = useEdge;
	}

	/// <summary>
	/// Edgeの色
	/// </summary>
	/// <param name="color"></param>
	inline void SetEdgeColor(Vector3 color) {
		this->dessolveValue_.edgeColor = color;
	}

	/// <summary>
	/// Edgeの厚さ
	/// </summary>
	/// <param name="thinkness"></param>
	inline void SetEdgeThinkness(float thinkness) {
		this->dessolveValue_.edgeThinkness = thinkness;
	}

	/// <summary>
	/// 閾値
	/// </summary>
	/// <param name="threshold"></param>
	inline void SetThreshold(float threshold) {
		this->dessolveValue_.threshold = threshold;
	}


private:
	struct DissolveData {
		//Edgeを使うかどうか
		bool isUseEdge;
		//Edgeの色
		Vector3 edgeColor;
		//Edgeの厚さ
		float edgeThinkness;
		//閾値
		float threshold;

	};
	ComPtr<ID3D12Resource> dissolveResource_ = nullptr;
	DissolveData dessolveValue_ = {};
	DissolveData* dissolveData_ = nullptr;

	//RTV
	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	uint32_t rtvHandle_ = 0;

	Vector4 renderTargetClearColor = {};
	uint32_t srvHandle_ = 0;
	uint32_t maskTextureHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier = {};
};

