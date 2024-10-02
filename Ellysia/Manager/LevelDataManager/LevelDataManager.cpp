#include "LevelDataManager.h"

#include <cassert>
#include <numbers>
#include <fstream>

#include "ModelManager.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

void LevelDataManager::Place(nlohmann::json& objects, LevelData& levelData) {

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : objects) {
		//各オブジェクトに必ずtypeが入っているよ
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESHの場合
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData.objectDatas.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData.objectDatas.back();
			//ここでのファイルネームはオブジェクトの名前
			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//Blenderと軸の方向が違うので注意！
			//平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];
			
			//回転角
			//そういえばBlenderは度数法だったね
			//エンジンでは弧度法に直そう
			const float DEREES_TO_RADIUS_ = (float)std::numbers::pi / 180.0f;
			objectData.rotation.x = -(float)transform["rotation"][1] * DEREES_TO_RADIUS_;
			objectData.rotation.y = -(float)transform["rotation"][2] * DEREES_TO_RADIUS_;
			objectData.rotation.z = -(float)transform["rotation"][0] * DEREES_TO_RADIUS_;

			//スケール
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];



			//コライダーの読み込み
			//まずあるかどうか
			if (object.contains("collider")) {
				nlohmann::json& collider = object["collider"];

				// コライダーの種別を取得
				if (collider.contains("type")) {
					objectData.colliderType = collider["type"];
				}

				//中心座標
				objectData.center.x = (float)collider["center"][1];
				objectData.center.y = (float)collider["center"][2];
				objectData.center.z = -(float)collider["center"][0];
				//サイズ
				objectData.size.x = (float)collider["size"][1];
				objectData.size.y = (float)collider["size"][2];
				objectData.size.z = (float)collider["size"][0];
			}

			//子オブジェクト
			if (object.contains("children")) {
				Place(object["children"], levelData);
			}

		}
	}

}

void LevelDataManager::Ganarate(LevelData& levelData) {

	std::string levelEditorDirectoryPath = "Resources/LevelData/"+ levelData.folderName;
	
	for (LevelData::ObjectData& objectData : levelData.objectDatas) {

		//モデルの生成
		//まだ無い場合は生成する
		if (objectData.model == nullptr) {
			//モデルの読み込み
			uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFileForLevelData(levelEditorDirectoryPath, objectData.fileName);
			//生成
			Model* model = Model::Create(modelHandle);
			//代入
			objectData.model = model;
		}
		
		//ワールドトランスフォームの初期化
		//空だったら生成する
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransform->translate_ = objectData.translation;
		worldTransform->rotate_ = objectData.rotation;
		worldTransform->scale_ = objectData.scaling;

		//配列に登録
		objectData.worldTransform = worldTransform;
		
	}
}



uint32_t LevelDataManager::Load(const std::string& filePath){

	//ファイルストリーム
	std::ifstream file;
	std::string fullFilePath;




	std::string levelEditorDirectoryPath = "Resources/LevelData/";
	fullFilePath = levelEditorDirectoryPath + filePath;



	
	file.open(fullFilePath);

	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	//objectかどうか
	assert(deserialized.is_object());
	//namaeのキーワードがあるかどうか
	assert(deserialized.contains("name"));
	//nameはstring型かどうか
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルはチェック
	assert(name.compare("scene") == 0);

	
	
	

	std::string folderName = {};
	std::string fileName = {};
	size_t slashPosition = filePath.find('/');

	if (slashPosition != std::string::npos) {
		//0からslashPositionまで
		folderName = filePath.substr(0, slashPosition);
		//「/」から最後まで
		fileName = filePath.substr(slashPosition + 1);
	}
	




	//レベルデータ格納用インスタンスを生成
	levelDatas_[fullFilePath] = std::make_unique<LevelData>();

	//それぞれに情報を記録
	levelDatas_[fullFilePath]->folderName = folderName;
	levelDatas_[fullFilePath]->fileName = fileName;
	levelDatas_[fullFilePath]->handle = handle_;
	levelDatas_[fullFilePath]->fullPath = fullFilePath;
	levelDatas_[fullFilePath]->object = deserialized;
	//ハンドルの加算
	++handle_;
	LevelData& levelData = *levelDatas_[fullFilePath];

	//読み込み
	Place(deserialized["objects"], levelData);

	//生成
	Ganarate(levelData);
	




	//番号を返す
	return levelDatas_[fullFilePath]->handle;

}

void LevelDataManager::Reload(uint32_t& levelDataHandle){
	levelDatas_;


	//一度全てのオブジェクトのデータを消す
	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		//LevelDataを取得
		LevelData* levelData = it->second.get();

		//一致したら消す
		if (levelData->handle == levelDataHandle) {
			//モデルを消す
			for (auto& object : levelData->objectDatas) {
				// Modelの解放
				if (object.model != nullptr) {
					delete object.model;
				}
				//ワールドトランスフォームの解放
				delete object.worldTransform;

			}

			levelData->objectDatas.~vector();

			//無駄な処理をしないようにする
			break;
		}
	}




	//引数から探す
	std::string fullFilePath = {};
	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get();
		if (levelData->handle == levelDataHandle) {
			fullFilePath = levelData->fullPath;
			break;
		}
	}


	std::ifstream file;
	file.open(fullFilePath);

	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	//objectかどうか
	assert(deserialized.is_object());
	//namaeのキーワードがあるかどうか
	assert(deserialized.contains("name"));
	//nameはstring型かどうか
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルはチェック
	assert(name.compare("scene") == 0);







	LevelData& levelData = *levelDatas_[fullFilePath];

	//読み込み
	Place(deserialized["objects"], levelData);

	//生成
	Ganarate(levelData);







}

void LevelDataManager::Update(uint32_t& levelDataHandle){
	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get(); 
		if (levelData->handle == levelDataHandle) {
			//更新
			for (auto& object : levelData->objectDatas) {
				object.worldTransform->Update();
			}
			break; 
		}
	}
}

#pragma region 描画

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, DirectionalLight& directionalLight){
	
	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get(); 
		if (levelData->handle == levelDataHandle) {

			//描画
			for (LevelData::ObjectData& object : levelData->objectDatas) {
				object.model->Draw(*object.worldTransform, camera, material, directionalLight);
			}
			//無駄なループ処理を防ぐよ
			break;

		}
	}
}

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, PointLight& pointLight){

	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get(); 
		if (levelData->handle == levelDataHandle) {

			//描画
			for (LevelData::ObjectData& object : levelData->objectDatas) {
				object.model->Draw(*object.worldTransform, camera, material, pointLight);
			}
			//無駄なループ処理を防ぐよ
			break;

		}
	}
}

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, SpotLight& spotLight){

	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get();
		if (levelData->handle == levelDataHandle) {

			//描画
			for (LevelData::ObjectData& object : levelData->objectDatas) {
				object.model->Draw(*object.worldTransform, camera, material, spotLight);
			}
			//無駄なループ処理を防ぐよ
			break;

		}
	}
}

#pragma endregion

LevelDataManager::~LevelDataManager(){
	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get(); 

		for (auto& object : levelData->objectDatas) {
			// Model の解放
			if (object.model != nullptr) {
				delete object.model;
			}
			delete object.worldTransform;

		}
	}

	levelDatas_.clear();
	
}