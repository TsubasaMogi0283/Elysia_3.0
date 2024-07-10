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

	

private:
	/// <summary>
	/// 再帰的な読み込み
	/// </summary>
	void RecursiveLoad(nlohmann::json& objects);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	void Initialize(const std::string& directoryPath);

public:

	/// <summary>
	/// レベルデータの読み込み
	/// </summary>
	/// <param name="directoryPath">ResourcesのLevelData</param>
	/// <param name="fileName">jsonファイル</param>
	void Load(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// レベルデータのグループを作る
	/// </summary>
	/// <param name="name"></param>
	void GenarateLevelData(const std::string& name);

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
			//ファイル名
			std::string fileName;
			//Transform
			Vector3 translation;
			Vector3 rotation;
			Vector3 scaling;

			//コライダー
			std::string colliderType;
			Vector3 center;
			Vector3 size;
		};

		//オブジェクト
		std::vector<ObjectData> objects;

		//ファイルパス
		std::string filePath;


	};


	//モデルのコンテナ
	std::map<std::string,Model*> models_;

	std::vector<WorldTransform*> worldTransforms_;
	LevelData* levelData = nullptr;
private:

	//ここにデータを入れていく
	std::map<std::string ,LevelData*> levelDatas_;






};

