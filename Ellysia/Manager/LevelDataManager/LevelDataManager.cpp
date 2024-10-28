#include "LevelDataManager.h"

#include <cassert>
#include <numbers>
#include <filesystem>
#include <iostream>

#include "ModelManager.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <Audio.h>


LevelDataManager* LevelDataManager::GetInstance(){
	static LevelDataManager instance;
	return &instance;
}



std::string LevelDataManager::FindExtension(const std::string& directory, const std::string& baseFileName) {
	
	//指定したディレクトリから目的のものを探す
	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.is_regular_file()) {
			std::string fileName = entry.path().stem().string(); 
			//見つかった場合
			if (fileName == baseFileName) {
				return entry.path().string();  
			}
		}
	}
	// 見つからなければ空文字を返す
	return {};  
}

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
				objectData.modelFileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//スケール
			objectData.transform.scale.x = (float)transform["scaling"][1];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][0];

			//回転角
			//そういえばBlenderは度数法だったね
			//弧度法に直そう
			const float DEREES_TO_RADIUS_ = (float)std::numbers::pi / 180.0f;
			objectData.transform.rotate.x = -(float)transform["rotation"][1] * DEREES_TO_RADIUS_;
			objectData.transform.rotate.y = -(float)transform["rotation"][2] * DEREES_TO_RADIUS_;
			objectData.transform.rotate.z = -(float)transform["rotation"][0] * DEREES_TO_RADIUS_;


			//Blenderと軸の方向が違うので注意！
			//平行移動
			objectData.transform.translate.x = (float)transform["translation"][1];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][0];
			
			
			//オブジェクトのタイプを取得
			nlohmann::json objectCondition = {};
			if (object.contains("object_type")) {
				objectCondition = object["object_type"];
			}


			//コライダーの読み込み
			//まずあるかどうか
			if (object.contains("collider")) {
				nlohmann::json& collider = object["collider"];

				// コライダーの種別を取得
				if (collider.contains("type")) {
					objectData.colliderType = collider["type"];
				}

				
				//BOX
				if (objectData.colliderType == "BOX") {
					//中心座標
					objectData.center.x = (float)collider["center"][1] + objectData.transform.translate.x;
					objectData.center.y = (float)collider["center"][2] + objectData.transform.translate.y;
					objectData.center.z = -(float)collider["center"][0] + objectData.transform.translate.z;
					//サイズ
					objectData.size.x = (float)collider["size"][1];
					objectData.size.y = (float)collider["size"][2];
					objectData.size.z = (float)collider["size"][0];

				}
				//AABB
				else if (objectData.colliderType == "AABB") {
					//中心座標
					objectData.center.x = (float)collider["center"][1] + objectData.transform.translate.x;
					objectData.center.y = (float)collider["center"][2] + objectData.transform.translate.y;
					objectData.center.z = -(float)collider["center"][0] + objectData.transform.translate.z;
					//サイズ
					objectData.size.x = (float)collider["size"][1];
					objectData.size.y = (float)collider["size"][2];
					objectData.size.z = (float)collider["size"][0];

					//右上奥
					objectData.upSize.x = objectData.center.x + objectData.size.x;
					objectData.upSize.y = objectData.center.y + objectData.size.y;
					objectData.upSize.z = objectData.center.z + objectData.size.z;
					//左下手前
					objectData.downSize.x = objectData.center.x - objectData.size.x;
					objectData.downSize.y = objectData.center.y - objectData.size.y;
					objectData.downSize.z = objectData.center.z - objectData.size.z;


				}

			}


			//オーディオの読み込み
			//まずあるかどうか
			if (objectCondition == "Audio") {
				if (object.contains("audio")) {
					nlohmann::json& audio = object["audio"];

					//Audioを持っているよ
					objectData.levelAudioData.isHavingAudio = true;

					//種類を記録
					if (audio.contains("type")) {
						objectData.levelAudioData.type = audio["type"];
					}

					//ファイル名を記録
					objectData.levelAudioData.fileName = audio["file_name"];

					//ループをするかどうか
					objectData.levelAudioData.isLoop = audio["loop"];

					//エリア上かどうか
					objectData.levelAudioData.isOnArea = audio["on_area"];


					//Audioフォルダの中で読み込み
					std::string audioDir = leveldataPath_ + levelData.folderName + "/Audio/" + objectData.levelAudioData.type + "/";
					std::string fullPath = FindExtension(audioDir, objectData.levelAudioData.fileName);
					objectData.levelAudioData.handle = Audio::GetInstance()->Load(fullPath);

				}
			}

			

			//子オブジェクト
			if (object.contains("children")) {
				Place(object["children"], levelData);
			}

		}
	}

}

void LevelDataManager::Ganarate(LevelData& levelData) {

	//ディレクトリパス
	std::string levelEditorDirectoryPath = leveldataPath_ + levelData.folderName;
	
	for (LevelData::ObjectData& objectData : levelData.objectDatas) {

		//モデルの生成
		//まだ無い場合は生成する
		if (objectData.model == nullptr) {
			//モデルの読み込み
			uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFileForLevelData(levelEditorDirectoryPath, objectData.modelFileName);
			//生成
			Model* model = Model::Create(modelHandle);
			//代入
			objectData.model = model;
		}

		//オーディオの場合
		if (objectData.levelAudioData.isHavingAudio == true) {

		}

		
		//ワールドトランスフォームの初期化
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransform->scale_ = objectData.transform.scale;
		worldTransform->rotate_ = objectData.transform.rotate;
		worldTransform->translate_ = objectData.transform.translate;

		//配列に登録
		objectData.worldTransform = worldTransform;
		
	}




}

nlohmann::json LevelDataManager::Deserialize(std::string& fullFilePath){
	std::ifstream file;
	//ファイルを開ける
	file.open(fullFilePath);

	//ミスしたら止める
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json data;

	//解凍
	file >> data;

	//正しいレベルデータファイルかチェック
	//objectかどうか
	assert(data.is_object());
	//namaeのキーワードがあるかどうか
	assert(data.contains("name"));
	//nameはstring型かどうか
	assert(data["name"].is_string());

	//"name"を文字列として取得
	std::string name = data["name"].get<std::string>();
	//正しいレベルデータファイルはチェック
	assert(name.compare("scene") == 0);


	//値を返す
	return data;

}

void LevelDataManager::AudioPlay(LevelData& levelData){
	for (LevelData::ObjectData& objectData : levelData.objectDatas) {

		//オーディオ機能を持っていたら通るよ
		if (objectData.levelAudioData.isHavingAudio == true) {
			//ハンドル
			uint32_t handle = objectData.levelAudioData.handle;
			//ループするかどうか
			bool isLoop = objectData.levelAudioData.isLoop;
			//再生
			Audio::GetInstance()->PlayWave(handle, isLoop);
		}
	}



}

uint32_t LevelDataManager::Load(const std::string& filePath){

	//パスの結合
	std::string fullFilePath = leveldataPath_ + filePath;

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized= Deserialize(fullFilePath);

	//ファイルパスの分解
	std::string folderName = {};
	std::string fileName = {};
	size_t slashPosition = filePath.find('/');

	if (slashPosition != std::string::npos) {
		//0からslashPositionまで
		folderName = filePath.substr(0, slashPosition);
		//「/」から最後まで
		fileName = filePath.substr(slashPosition + 1);
	}
	

	//インスタンスを生成
	levelDatas_[fullFilePath] = std::make_unique<LevelData>();

	//それぞれに情報を記録
	levelDatas_[fullFilePath]->folderName = folderName;
	levelDatas_[fullFilePath]->fileName = fileName;
	levelDatas_[fullFilePath]->handle = handle_;
	levelDatas_[fullFilePath]->fullPath = fullFilePath;

	//ハンドルの加算
	++handle_;

	LevelData& levelData = *levelDatas_[fullFilePath];

	//読み込み
	Place(deserialized["objects"], levelData);

	//生成
	Ganarate(levelData);
	
	//オーディオの再生
	//AudioPlay(levelData);



	//番号を返す
	return levelDatas_[fullFilePath]->handle;

}

void LevelDataManager::Reload(uint32_t& levelDataHandle){


	//一度全てのオブジェクトのデータを消す
	for (auto& [key, levelDataPtr] : levelDatas_) {
		if (levelDataPtr->handle == levelDataHandle) {

			//モデルを消す
			for (auto& object : levelDataPtr->objectDatas) {
				// Modelの解放
				if (object.model != nullptr) {
					delete object.model;
				}
				//ワールドトランスフォームの解放
				delete object.worldTransform;

			}

			//listにある情報を全て消す
			levelDataPtr->objectDatas.clear();

			//無駄な処理をしないようにする
			break;
		}
	}



	//引数から探す
	std::string fullFilePath = {};
	for (std::map<std::string, std::unique_ptr<LevelData>>::iterator it = levelDatas_.begin(); it != levelDatas_.end(); ++it) {
		LevelData* levelData = it->second.get();

		//見つけたらfullFilePathに入れる
		if (levelData->handle == levelDataHandle) {
			fullFilePath = levelData->fullPath;
			break;
		}
	}



	//JSON文字列から解凍したデータ
	nlohmann::json deserialized= Deserialize(fullFilePath);



	//元々あったデータを取得する
	//オブジェクトは全部消したけどここにファイルパスの情報が残っているよ
	LevelData& levelData = *levelDatas_[fullFilePath];

	//読み込み
	Place(deserialized["objects"], levelData);

	//生成
	Ganarate(levelData);

}

void LevelDataManager::Update(uint32_t& levelDataHandle){

	//この書き方はC++17からの構造化束縛というものらしい
	//イテレータではなくこっちでやった方が良いかな
	//ファイル名で指定したい時はkeyを使ったら楽だね。今回はハンドルだけどね。
	for (auto& [key, levelData] : levelDatas_) {
		if (levelData->handle == levelDataHandle) {
			
			for (auto& object : levelData->objectDatas) {
				//更新
				object.worldTransform->Update();
			}
			break; 
		}
	}
}

void LevelDataManager::Delete(uint32_t& levelDataHandle){


	for (auto& [key, levelData] : levelDatas_) {
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

			//listにある情報を全て消す
			levelData->objectDatas.clear();

			//無駄な処理をしないようにする
			break;
		}
	}
}

#pragma region 描画

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, DirectionalLight& directionalLight){
	
	//指定したハンドルのデータだけを描画
	for (auto& [key, levelData] : levelDatas_) {
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

	//指定したハンドルのデータだけを描画
	for (auto& [key, levelData] : levelDatas_) {
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
	
	//指定したハンドルのデータだけを描画
	for (auto& [key, levelData] : levelDatas_) {
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


void LevelDataManager::Release(){

	//全て解放
	for (auto& [key, levelData] : levelDatas_) {
		for (auto& object : levelData->objectDatas) {
			// Model の解放
			if (object.model != nullptr) {
				delete object.model;
			}
			delete object.worldTransform;

		}

	}

	//クリア
	levelDatas_.clear();
}


