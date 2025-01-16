#pragma once

/**
 * @file SrvManager.h
 * @brief SRV管理クラス
 * @author 茂木翼
 */

#include "DirectXSetup.h"

/// <summary>
/// EllysiaEngine
/// </summary>
namespace Ellysia {

	/// <summary>
	/// SRV管理クラス
	/// </summary>
	class SrvManager final {
	private:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		SrvManager() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~SrvManager() = default;

	public:
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static SrvManager* GetInstance();

		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		/// <param name="srvHeapManager"></param>
		SrvManager(const SrvManager& srvHeapManager) = delete;

		/// <summary>
		/// 代入演算子を無効にする
		/// </summary>
		/// <param name="rvHeapManager"></param>
		/// <returns></returns>
		SrvManager& operator=(const SrvManager& rvHeapManager) = delete;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 加算していく
		/// </summary>
		/// <returns></returns>
		uint32_t Allocate();

		/// <summary>
		/// 描画前処理
		/// </summary>
		void PreDraw();

		/// <summary>
		/// 2Dテクスチャ用のSRVを作成
		/// </summary>
		/// <param name="srvIndex"></param>
		/// <param name="pResource"></param>
		/// <param name="format"></param>
		/// <param name="mipLevels"></param>
		/// <param name="isCubeMap"></param>
		void CreateSRVForTexture2D(const uint32_t& srvIndex, ID3D12Resource* pResource, const  DXGI_FORMAT& format, const UINT& mipLevels, const bool& isCubeMap);

		/// <summary>
		/// StructureBuffer用のSRVを作成
		/// </summary>
		/// <param name="srvIndex"></param>
		/// <param name="pResource"></param>
		/// <param name="numElements"></param>
		/// <param name="structureByteStride"></param>
		void CreateSRVForStructuredBuffer(const uint32_t& srvIndex, ID3D12Resource* pResource, const UINT& numElements, const UINT& structureByteStride);

		/// <summary>
		/// RenderTexture用のSRVを作成
		/// </summary>
		/// <param name="pResource"></param>
		/// <param name="handle"></param>
		void CreateSRVForRenderTexture(ID3D12Resource* pResource, const uint32_t& handle);

		/// <summary>
		/// DepthTexture用のSRVを作成
		/// </summary>
		/// <param name="handle"></param>
		void CreateSRVForDepthTexture(const uint32_t& handle);



		/// <summary>
		/// ディスクリプタテーブルの設定
		/// </summary>
		/// <param name="rootParameterIndex"></param>
		/// <param name="srvIndex"></param>
		void SetGraphicsRootDescriptorTable(const UINT& rootParameterIndex, const uint32_t& srvIndex);


	public:
		/// <summary>
		/// CPUのハンドルを取得
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const uint32_t& index) {
			D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
			handleCPU.ptr += (descriptorSize_ * index);
			return handleCPU;
		};

		/// <summary>
		/// GPUのハンドルを取得
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const uint32_t& index) {
			D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
			handleGPU.ptr += (descriptorSize_ * index);
			return handleGPU;
		};

		/// <summary>
		/// ディスクリプタの取得
		/// </summary>
		/// <returns></returns>
		inline ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() const {
			return  descriptorHeap_;
		}

	private:
		//最大SRV数(最大テクスチャ数)
		const uint32_t MAX_SRV_COUNT_ = 512;

		//SRV用のデスクリプタサイズ
		uint32_t descriptorSize_ = 0;

		//SRV用デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;

		//ハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU_ = {};
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU_ = {};

		//次に使用するSRVインデックス
		uint32_t useIndex_ = 1;



	};

}