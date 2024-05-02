#include "LevelDataManager.h"
#include <cassert>
#include <json.hpp>
#include <fstream>

#include "ModelManager.h"
#include "Camera.h"


void LevelDataManager::Load(std::string filePath){
#pragma region 後々クラス化する
	const std::string fullpath = "Resources/LevelData/TL1Test.json";
	const std::string fileP = filePath;
	fileP;
	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);

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

	//レベルデータ格納用インスlタンスを生成
	levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		//各オブジェクトに必ずtypeが入っているよ
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESHの場合
		if (type.compare("MESH") == 0) {
			//要素追加
			//emplace_backというとvectorだね！
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			//Blenderと軸の方向が違うので注意！

			//  自作エンジン      Blender
			//		x		←		y
			//		y		←		z
			//		z		←		-x
			//
			//回転
			//進行方向に向かって...
			//     左回り		右回り
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//コライダーの読み込み

		}


		//再帰関数
		//Python側でやったやつを参考にしてね
		if (object.contains("children")) {

		}


	}


	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");

	for (auto& objectData : levelData->objects) {
		//first,secondとあるからmapかも
		decltype(models_)::iterator it = models_.find(objectData.fileName);


		//まだ読み込みがされていない場合読み込む
		if (it == models_.end()) {
			Model* model = nullptr;
			model=Model::Create(modelHandle);
			models_[objectData.fileName] = model;
		}
		
		//座標を入れるのWorldTransformしかないのでそれでやる
		WorldTransform* worldTransform = new WorldTransform();

		worldTransform->Initialize();
		worldTransform->translate_ = objectData.translation;
		worldTransform->rotate_ = objectData.rotation;
		worldTransform->scale_ = objectData.scaling;



		//配列に登録
		//vector list?
		worldTransforms_.push_back(worldTransform);
	}

	//Modelに入れるWorldTransformを生成する
	/*for (auto& objectData : levelData->objects) {

		
	}*/


#pragma endregion
}

void LevelDataManager::Configure(){

}

void LevelDataManager::Update(){
	for (WorldTransform* object : worldTransforms_) {
		object->Update();
	}
}

void LevelDataManager::Draw(Camera& camera){
	uint32_t count = 0;
	for (auto& objectData : levelData->objects) {
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		//first,secondとあるからmapかも
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		//見つかったらmodelに入れる
		if (it != models_.end()) {
			model = it->second;
		}

		model->Draw(*worldTransforms_[count], camera);


		count++;
	}
}

LevelDataManager::~LevelDataManager(){
	models_.clear();
	worldTransforms_.clear();
	delete levelData;
}