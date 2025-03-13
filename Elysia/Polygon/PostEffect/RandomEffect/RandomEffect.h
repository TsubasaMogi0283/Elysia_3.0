#pragma once

/**
 * @file RandomEffect.h
 * @brief ランダムエフェクトのクラス
 * @author 茂木翼
 */

#include <time.h>
#include <random>

#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector3.h"
#include "VertexData.h"



 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {
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
	/// ランダムエフェクト
	/// </summary>
	class RandomEffect {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		RandomEffect();

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
		~RandomEffect() = default;

	private:
		//Windowクラス
		Elysia::WindowsSetup* windowSetup_ = nullptr;
		//DirectXクラス
		Elysia::DirectXSetup* directXSetup_ = nullptr;
		//パイプライン管理クラス
		Elysia::PipelineManager* pipelineManager_ = nullptr;
		//RTV管理クラス
		Elysia::RtvManager* rtvManager_ = nullptr;
		//SRV管理クラス
		Elysia::SrvManager* srvManager_ = nullptr;


	private:
		/// <summary>
		/// ランダムの値
		/// </summary>
		struct RandomValue {
			//値
			float value;
			//テクスチャと乗算させるか
			bool isUseTexture;
		};

	private:
		//ランダム
		//リソース
		ComPtr<ID3D12Resource> randomValueResource_ = nullptr;
		//値
		RandomValue randomValue_ = {};
		//データ
		RandomValue* randomValueData_ = nullptr;
		//エンジン
		std::mt19937 randomEngine_ = {};

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



};
