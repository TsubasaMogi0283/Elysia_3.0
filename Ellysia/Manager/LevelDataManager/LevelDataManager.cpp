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
			//emplace_backというとvectorだね！
			levelData.objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData.objects.back();
			//ここでのファイルネームはオブジェクトの名前
			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			



			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//Blenderと軸の方向が違うので注意！
			// 
			//平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];
			
			//回転角
			//そういえばBlenderは度数法だったね
			//エンジンでは弧度法に直そう
			const float DEREES_TO_RADIUS_ = (float)std::numbers::pi / 180.0f;
			objectData.rotation.x = -(float)transform["rotation"][1]*DEREES_TO_RADIUS_;
			objectData.rotation.y = -(float)transform["rotation"][2]*DEREES_TO_RADIUS_;
			objectData.rotation.z = -(float)transform["rotation"][0]* DEREES_TO_RADIUS_;

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
					objectData.colliderType = collider["type"];  // ここで正しいコライダータイプを取得
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

			if (object.contains("children")) {
				Place(object["children"], levelData);
			}

		}
	}

}

void LevelDataManager::Ganarate(LevelData& levelData) {

	std::string levelEditorDirectoryPath = "Resources/LevelData/"+ levelData.folderName;



	for (auto& objectData : levelData.objects) {
		//first,secondとあるからmapかも
		decltype(models_)::iterator it = models_.find(objectData.fileName);

		//まだ読み込みがされていない場合読み込む
		if (it == models_.end()) {
			Model* model = nullptr;
			std::string modelPath = objectData.fileName ;
			uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFileForLevelData(levelEditorDirectoryPath, modelPath);
			model = Model::Create(modelHandle);
			models_[objectData.fileName] = model;
		}


		//モデル

		//levelDatas_[filePathString]




		//ワールドトランスフォームの初期化
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransform->translate_ = objectData.translation;
		worldTransform->rotate_ = objectData.rotation;
		worldTransform->scale_ = objectData.scaling;



		//配列に登録
		worldTransforms_.push_back(worldTransform);
	}
}


void LevelDataManager::Load(const std::string& filePath){

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	std::string levelEditorDirectoryPath = "Resources/LevelData/";
	std::string filePathString = levelEditorDirectoryPath + filePath ;
	file.open(filePathString);

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

	//レベルデータ格納用インスタンスを生成
	levelDatas_[filePathString] = new LevelData();
	
	

	std::string folderName = {};
	std::string fileName = {};
	size_t slashPosition = filePath.find('/');

	if (slashPosition != std::string::npos) {
		//0からslashPositionまで
		folderName = filePath.substr(0, slashPosition);
		//「/」から最後まで
		fileName = filePath.substr(slashPosition + 1);
	}
	

	//それぞれに情報を記録
	levelDatas_[filePathString]->folderName = folderName;
	levelDatas_[filePathString]->fileName = fileName;
	levelDatas_[filePathString]->handle = handle_;

	LevelData& levelData = *levelDatas_[filePathString];

	//ハンドルの加算
	++handle_;

	//読み込み(再帰機能付き)
	Place(deserialized["objects"], levelData);
	
	//生成
	Ganarate(levelData);

}

void LevelDataManager::Update(uint32_t& levelDataHandle){
	levelDataHandle;
	//ワールドトランスフォームの更新
	for (WorldTransform* object : worldTransforms_) {
		object->Update();
	}

}

#pragma region 描画

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, DirectionalLight& directionalLight){
	levelDataHandle;
	
	uint32_t count = 0;
	for (auto& objectData : levelDatas_.begin()->second->objects) {
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		//見つかったらmodelに入れる
		if (it != models_.end()) {
			model = it->second;
		}

		model->Draw(*worldTransforms_[count], camera, material, directionalLight);

		//数を増やしていく
		count++;
	}
}

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, PointLight& pointLight){
	levelDataHandle;
	
	uint32_t count = 0;
	for (auto& objectData : levelDatas_.begin()->second->objects) {
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		//見つかったらmodelに入れる
		if (it != models_.end()) {
			model = it->second;
		}

		model->Draw(*worldTransforms_[count], camera, material, pointLight);

		//数を増やしていく
		count++;
	}
}

void LevelDataManager::Draw(uint32_t& levelDataHandle, Camera& camera, Material& material, SpotLight& spotLight){
	levelDataHandle;
	
	uint32_t count = 0;
	for (auto& objectData : levelDatas_.begin()->second->objects) {
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		//見つかったらmodelに入れる
		if (it != models_.end()) {
			model = it->second;
		}

		model->Draw(*worldTransforms_[count], camera, material, spotLight);

		//数を増やしていく
		count++;
	}
}

#pragma endregion

LevelDataManager::~LevelDataManager(){
	for (auto& pair : models_) {
		delete pair.second;
	}
	models_.clear();

	for (WorldTransform* object : worldTransforms_) {
		delete object;
	}
	worldTransforms_.clear();

	for (auto& pair : levelDatas_) {
		delete pair.second;
	}
	levelDatas_.clear();
}