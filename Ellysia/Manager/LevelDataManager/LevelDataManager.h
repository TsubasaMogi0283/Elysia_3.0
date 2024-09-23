#pragma once
#include <string>
#include <Vector3.h>
#include <vector>
#include <array>
#include <map>
#include <json.hpp>
#include <Model.h>
#include "WorldTransform.h"

//このクラスを元に継承させた方が良いかも
//Modelは別々のものだから


struct Camera;
struct Material;
struct DirectionalLight;
struct PointLight;
struct SpotLight;

class LevelDataManager {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelDataManager()=default;

	


public:

	/// <summary>
	/// レベルデータの読み込み
	/// </summary>
	/// <param name="filePath">jsonファイル</param>
	void Load(const std::string& filePath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画(平行光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	void Draw(Camera& camera, Material& material, DirectionalLight& directionalLight);

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="pointLight"></param>
	void Draw(Camera& camera, Material& material, PointLight& pointLight);

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,Material & material,SpotLight& spotLight);




	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelDataManager();




private:
	

	struct LevelData {
		struct ObjectData {
			//オブジェクトのファイル名
			std::string fileName;
			//Transform
			Vector3 scaling;
			Vector3 rotation;
			Vector3 translation;

			//コライダー
			std::string colliderType;
			Vector3 center;
			Vector3 size;
		};

		//オブジェクト
		std::vector<ObjectData> objects;

		//ファイルパス
		std::string filePath;

		//フォルダ名
		std::string folderName;
		//ファイル名
		std::string fileName;


	};


	//モデルのコンテナ
	std::map<std::string,Model*> models_;

	std::vector<WorldTransform*> worldTransforms_;
	//LevelData* levelData = nullptr;

private:
	/// <summary>
	/// 位置決め
	/// </summary>
	void Place(nlohmann::json& objects, LevelData& levelData);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	void Ganarate(LevelData& levelData);


private:

	//ここにデータを入れていく
	std::map<std::string ,LevelData*> levelDatas_;

};

