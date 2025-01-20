#pragma once
/**
 * @file GrayScale.h
 * @brief グレースケール
 * @author 茂木翼
 */


#include "DirectXSetup.h"



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
	/// グレースケール
	/// </summary>
	class GrayScale {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GrayScale();

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
		~GrayScale() = default;

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
		//RTV
		//リソース
		ComPtr<ID3D12Resource> rtvResource_ = nullptr;
		//ハンドル
		uint32_t rtvHandle_ = 0;

		//SRVハンドル
		uint32_t srvHandle_ = 0;
		//バリア
		D3D12_RESOURCE_BARRIER barrier = {};


	};

}