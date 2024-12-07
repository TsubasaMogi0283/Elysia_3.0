#pragma once

/**
 * @file TextureManager.h
 * @brief テクスチャ管理クラスのクラス
 * @author 茂木翼
 */

#include <d3d12.h>
#include <string>
#include <array>
#include <DirectXTex.h>
#include <d3dx12.h>
#include <map>

#include "DirectXSetup.h"
#include "ConvertLog.h"

/// <summary>
/// テクスチャ管理クラス
/// </summary>
class TextureManager final{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureManager() = default;

public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="textureManager"></param>
	TextureManager(const TextureManager& textureManager) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="textureManager"></param>
	/// <returns></returns>
	TextureManager& operator=(const TextureManager& textureManager) = delete;


public:

	//統合させた関数
	
	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static uint32_t LoadTexture(const std::string& filePath);

	/// <summary>
	/// コマンドを送る
	/// </summary>
	/// <param name="rootParameter"></param>
	/// <param name="textureHandle"></param>
	static void GraphicsCommand(const uint32_t& rootParameter,const uint32_t& textureHandle);



	/// <summary>
	/// DESCの取得
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <returns></returns>
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

private:

#pragma region テクスチャの読み込み
	
	//1.TextureデータそのものをCPUで読み込む
	//2.DirectX12のTextureResourceを作る
	//3.TextureResourceに1で読んだデータを転送する

	/// <summary>
	/// Textureデータの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static DirectX::ScratchImage LoadTextureData(const std::string& filePath);

	

	/// <summary>
	/// リソースを作る
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// データの転送
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> TransferTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);


#pragma endregion


public:

	/// <summary>
	/// テクスチャ情報
	/// </summary>
	struct TextureInformation {

		//リソース
		ComPtr<ID3D12Resource> resource_ = nullptr;
		ComPtr<ID3D12Resource> internegiateResource_ = nullptr;

		DirectX::ScratchImage mipImages_ = {};
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_ = {};


		//読み込んだテクスチャの名前
		std::string name_ = {};

		//テクスチャハンドル
		uint32_t handle_ = 0;
	};


	/// <summary>
	/// テクスチャ情報を取得/設定する
	/// </summary>
	/// <returns></returns>
	inline std::map<std::string, TextureInformation>& GetTextureInformation() {
		return textureInformation_;
	}

private:
	//インデックス
	static uint32_t index_;

	//テクスチャ情報
	std::map<std::string, TextureInformation> textureInformation_{};
	
	// handleからfilePathへのマッピングを保持する
	std::map<uint32_t, std::string> handleToFilePathMap_{};

};