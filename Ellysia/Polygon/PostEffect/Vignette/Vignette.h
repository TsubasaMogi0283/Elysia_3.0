#pragma once
/**
 * @file Vignette.h
 * @brief ビネットのクラス
 * @author 茂木翼
 */

#include "DirectXSetup.h"
#include "Vector3.h"

/// <summary>
/// ベクトル(4次元)
/// </summary>
struct Vector4;

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
	/// ビネット
	/// </summary>
	class Vignette {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Vignette();

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
			this->vignetteValue_.scale = scale;
		}

		/// <summary>
		/// 何乗するかの設定
		/// </summary>
		/// <param name="pow"></param>
		inline void SetPow(const float& pow) {
			this->vignetteValue_.pow = pow;
		}

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color"></param>
		inline void SetColor(const Vector3& color) {
			this->vignetteValue_.color = color;
		}

	private:
		//送るデータ
		struct VignetteData {
		public:
			//倍
			float scale;
			//乗
			float pow;
			//色
			Vector3 color;

		private:
			float padding[3];

		};

	private:
		//ウィンドウクラス
		Ellysia::WindowsSetup* windowsSetup_ = nullptr;
		//DirectXクラス
		Ellysia::DirectXSetup* directXSetup_ = nullptr;
		//パイプライン管理クラス
		Ellysia::PipelineManager* pipelinemanager_ = nullptr;
		//RTV管理クラス
		Ellysia::RtvManager* rtvManager_ = nullptr;
		//SRV管理クラス
		Ellysia::SrvManager* srvManager_ = nullptr;

	private:
		//ハンドル
		uint32_t rtvHandle_ = 0;
		//RTV
		ComPtr<ID3D12Resource> rtvResource_ = nullptr;
		//SRV
		uint32_t srvHandle_ = 0;

		//リソース
		ComPtr<ID3D12Resource> valueResource_ = nullptr;
		//送るデータ
		VignetteData* vignetteData_ = nullptr;
		//値
		VignetteData vignetteValue_ = {};

		//リソースバリア
		D3D12_RESOURCE_BARRIER barrier = {};

	};

};