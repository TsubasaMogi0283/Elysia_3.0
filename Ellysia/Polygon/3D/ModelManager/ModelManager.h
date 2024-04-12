#pragma once
#include <string>
#include <array>
#include <ModelData.h>
#include <fstream>
#include <sstream>
#include <list>



class ModelManager final{
private:
	ModelManager();
	
	~ModelManager();

public:
	//シングルインスタンス
	static ModelManager* GetInstance();

	//コピーコンストラクタ禁止
	ModelManager(const ModelManager& modelManager) = delete;

	//代入演算子を無効にする
	ModelManager& operator=(const ModelManager& modelManager) = delete;




public:

	ModelData GetModelData(uint32_t handle) {
		return modelInfromtion_[handle].data;
	}


	//モデルデータの読み込み(ハンドルを登録する)
	//本体はここで使う
	static uint32_t LoadModelFile(const std::string& directoryPath, const std::string& fileName);


private:

	//モデルデータの読み込み(本体)
	static ModelData LoadFile(const std::string& directoryPath, const std::string& fileName);





private:

	//読み込みの最大数
	static const uint32_t MODEL_MAX_AMOUNT_ = 512;

	struct ModelInformation {
		//モデルデータ
		ModelData data;
		//ハンドル
		uint32_t handle;

		//ファイル名など
		std::string directoryPath;
		std::string filePath;
	};

	//数の限界があるのでarrayがいいかな
	std::array<ModelInformation, MODEL_MAX_AMOUNT_> modelInfromtion_{};

};
