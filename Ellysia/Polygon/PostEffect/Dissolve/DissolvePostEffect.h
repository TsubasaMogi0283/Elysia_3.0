#pragma once

/**
 * @file DissolvePostEffect.h
 * @brief ディゾルブ(ポストエフェクト)のクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"
#include "Dissolve.h"

/// <summary>
/// ディゾルブ
/// </summary>
class DissolvePostEffect {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DissolvePostEffect()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="maskTexture"></param>
	void Initialize(const uint32_t& maskTexture);

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
	~DissolvePostEffect()=default;

public:
	/// <summary>
	/// Edgeを使うかどうか
	/// </summary>
	/// <param name="useEdge"></param>
	inline void SetUseEdge(const bool& useEdge) {
		this->dessolveValue_.isUseEdge = useEdge;
	}

	/// <summary>
	/// Edgeの色
	/// </summary>
	/// <param name="color"></param>
	inline void SetEdgeColor(const Vector3& color) {
		this->dessolveValue_.edgeColor = color;
	}

	/// <summary>
	/// Edgeの厚さ
	/// </summary>
	/// <param name="thinkness"></param>
	inline void SetEdgeThinkness(const float& thinkness) {
		this->dessolveValue_.edgeThinkness = thinkness;
	}

	/// <summary>
	/// 閾値
	/// </summary>
	/// <param name="threshold"></param>
	inline void SetThreshold(const float& threshold) {
		this->dessolveValue_.threshold = threshold;
	}


private:

	
	//リソース
	ComPtr<ID3D12Resource> dissolveResource_ = nullptr;
	//値
	DissolveData dessolveValue_ = {};
	//データ
	DissolveData* dissolveData_ = nullptr;

	//RTV
	ComPtr<ID3D12Resource> rtvResource_ = nullptr;
	//RTVハンドル
	uint32_t rtvHandle_ = 0;
	//レンダーの色
	Vector4 renderTargetClearColor = {};
	//SRVハンドル
	uint32_t srvHandle_ = 0;
	//マスクのテクスチャハンドル
	uint32_t maskTextureHandle_ = 0;
	//バリア
	D3D12_RESOURCE_BARRIER barrier = {};
};

