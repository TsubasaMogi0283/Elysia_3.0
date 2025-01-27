#pragma once

/**
 * @file Dissolve.h
 * @brief ディゾルブのクラス
 * @author 茂木翼
 */


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"


/// <summary>
/// データ
/// </summary>
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

/// <summary>
/// EllysiaEngine
/// </summary>
namespace Ellysia {
	/// <summary>
	/// ウィンドウクラス
	/// </summary>
	class WindowsSetup;

	/// <summary>
	/// DirectXクラス
	/// </summary>
	class DirectXSetup;

	/// <summary>
	/// パイプライン管理クラス
	/// </summary>
	class PipelineManager;

	/// <summary>
	/// RTV管理クラス
	/// </summary>
	class RtvManager;

	/// <summary>
	/// SRV管理クラス
	/// </summary>
	class SrvManager;

	/// <summary>
	/// ディゾルブ
	/// </summary>
	class DissolveEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		DissolveEffect();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="clearColor"></param>
		void Initialize(const Vector4& clearColor);

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
		~DissolveEffect() = default;

	public:
		/// <summary>
		/// マスクテクスチャの設定
		/// </summary>
		/// <param name="maskTextureHandle">テクスチャハンドル</param>
		inline void SetMaskTexture(const uint32_t& maskTextureHandle) {
			this->maskTextureHandle_ = maskTextureHandle;
		}

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
		//Windowクラス
		Ellysia::WindowsSetup* windowSetup_ = nullptr;
		//DirectXクラス
		Ellysia::DirectXSetup* directXSetup_ = nullptr;
		//パイプライン管理クラス
		Ellysia::PipelineManager* pipelineManager_ = nullptr;
		//RTV管理クラス
		Ellysia::RtvManager* rtvManager_ = nullptr;
		//SRV管理クラス
		Ellysia::SrvManager* srvManager_ = nullptr;

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
		Vector4 renderTargetClearColor_ = {};
		//SRVハンドル
		uint32_t srvHandle_ = 0;
		//マスクのテクスチャハンドル
		uint32_t maskTextureHandle_ = 0;
		//バリア
		D3D12_RESOURCE_BARRIER barrier = {};
	};


}