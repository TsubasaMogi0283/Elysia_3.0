#pragma once
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <list>
#include <map>

#include <ModelData.h>
#include "Animation.h"

class ModelManager final{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelManager()=default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelManager()=default;

public:
	//シングルインスタンス
	static ModelManager* GetInstance();

	//コピーコンストラクタ禁止
	ModelManager(const ModelManager& modelManager) = delete;

	//代入演算子を無効にする
	ModelManager& operator=(const ModelManager& modelManager) = delete;


private:

	/// <summary>
	/// 通常のモデルデータの読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static ModelData LoadFile(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// レベルエディタ専用のモデルデータの読み込み
	/// </summary>
	/// <param name="fileNameFolder"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static ModelData LoadFileForLeveldata(const std::string& fileNameFolder, const std::string& fileName);

public:



	/// <summary>
	/// モデルデータの読み込み(ハンドルを登録する)
	/// </summary>
	/// <param name="directoryPath">パス</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static uint32_t LoadModelFile(const std::string& directoryPath, const std::string& fileName);


	/// <summary>
	/// モデルデータの読み込み
	/// </summary>
	static uint32_t LoadModelFileForLevelData(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// アニメーション付きのglTFを読み込みたいときはこっちで
	/// falseにすればobjも読み込めるよ
	/// </summary>
	/// <param name="directoryPath">パス</param>
	/// <param name="fileName">ファイル名</param>
	/// <param name="isAnimationLoad">アニメーションを読み込むかどうか</param>
	/// <returns></returns>
	static uint32_t LoadModelFile(const std::string& directoryPath, const std::string& fileName,bool isAnimationLoad);





public:

	//ModelData GetModelData(const std::string& filePath) {
	//	auto it = modelInfromtion_.find(filePath);
	//	if (it != modelInfromtion_.end()) {
	//		return it->second.modelData;
	//	}
	//	// キーが存在しない場合のエラー処理
	//	throw std::runtime_error("Model not found");
	//}
	//
	//Animation GetModelAnimation(const std::string& filePath) {
	//	auto it = modelInfromtion_.find(filePath);
	//	if (it != modelInfromtion_.end()) {
	//		return it->second.animationData;
	//	}
	//	// キーが存在しない場合のエラー処理
	//	throw std::runtime_error("Model not found");
	//}


	ModelData GetModelData(uint32_t handle) {
		for (const auto& [filePath, modelInfo] : modelInfromtion_) {
			if (modelInfo.handle == handle) {
				return modelInfo.modelData;
			}
		}
		// 一致するhandleが見つからなかった場合のエラー処理
		throw std::runtime_error("Model not found for given handle");
	}

	Animation GetModelAnimation(uint32_t handle) {
		for (const auto& [filePath, modelInfo] : modelInfromtion_) {
			if (modelInfo.handle == handle) {
				return modelInfo.animationData;
			}
		}
		// 一致するhandleが見つからなかった場合のエラー処理
		throw std::runtime_error("Animation not found for given handle");
	}


private:

	struct ModelInformation {
		//モデルデータ
		ModelData modelData;
		//アニメーション
		Animation animationData;

		//ハンドル
		uint32_t handle;

		//ファイル名など
		std::string directoryPath;
		std::string filePath;

		//レベルデータ用
		std::string folderName;
	};



	//ここにどんどんデータを入れていく
	std::map<std::string, ModelInformation> modelInfromtion_{};
};
