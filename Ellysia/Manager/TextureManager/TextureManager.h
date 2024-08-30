#pragma once
#include <d3d12.h>
#include <string>
#include <array>
#include <DirectXTex.h>
#include <d3dx12.h>


#include "DirectXSetup.h"
#include "ConvertLog.h"

//テクスチャに関するクラス
class TextureManager {
private:
	//コンストラクタ
	TextureManager()=default;

	//コンストラクタ
	~TextureManager()=default;

public:
	static TextureManager* GetInstance();

	//コピーコンストラクタ禁止
	TextureManager(const TextureManager& textureManager) = delete;

	//代入演算子を無効にする
	TextureManager& operator=(const TextureManager& textureManager) = delete;


public:
	
	//統合させた関数
	//インデックスを返すからマイナスはありえない。
	//uintにしたほうが良いよね
	static uint32_t LoadTexture(const std::string& filePath);

	static void GraphicsCommand(uint32_t rootParameter, uint32_t texHandle);



	/// テクスチャの情報を取得
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

private:




#pragma region テクスチャの読み込み
	//Textureデータを読む
	//1.TextureデータそのものをCPUで読み込む
	static DirectX::ScratchImage LoadTextureData(const std::string& filePath);

	//2.DirectX12のTextureResourceを作る
	static ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	//void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages,ID3D12Device* device,ID3D12GraphicsCommandList* commandList);
	static ComPtr<ID3D12Resource> UploadTextureData(ComPtr<ID3D12Resource> texture,const DirectX::ScratchImage& mipImages);


#pragma endregion
	

public:
	static const int TEXTURE_MAX_AMOUNT_ = 256;



	struct TextureInformation {

		//リソース
		ComPtr<ID3D12Resource> resource_= nullptr;
		ComPtr<ID3D12Resource> internegiateResource_ = nullptr;
		//画像読み込み
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_ = {};
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_ = {};

		//読み込んだテクスチャの名前
		std::string name_={};

		//テクスチャハンドル
		uint32_t handle_ = 0;
	};

private:



	//構造体版
	//mapにしたい
	//array...stdの配列版。その名前の通り配列だよね
	std::array<TextureInformation, TEXTURE_MAX_AMOUNT_> textureInformation_{};



};