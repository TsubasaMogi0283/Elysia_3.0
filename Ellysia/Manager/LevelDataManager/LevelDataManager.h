#pragma once
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>

#include <Vector3.h>
#include <json.hpp>
#include <Model.h>
#include "WorldTransform.h"
#include "Collider/Collider.h"


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
	uint32_t Load(const std::string& filePath);

	/// <summary>
	/// 再読み込み
	/// </summary>
	/// <param name="levelDataHandle"></param>
	void Reload(uint32_t& levelDataHandle);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="levelDataHandle"></param>
	void Update(uint32_t&levelDataHandle);

#pragma region 描画

	/// <summary>
	/// 描画(平行光源)
	/// </summary>
	/// <param name="levelDataHandle"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	void Draw(uint32_t& levelDataHandle,Camera& camera, Material& material, DirectionalLight& directionalLight);

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="levelDataHandle"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="pointLight"></param>
	void Draw(uint32_t& levelDataHandle,Camera& camera, Material& material, PointLight& pointLight);

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="levelDataHandle"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(uint32_t& levelDataHandle,Camera& camera,Material & material,SpotLight& spotLight);


#pragma endregion

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelDataManager();




private:
	

	struct LevelData {
		struct ObjectData {
			//モデル
			Model* model = nullptr;
			//ワールドトランスフォーム
			WorldTransform worldTransform = {};

			//オブジェクトのファイル名
			std::string fileName;
			
			//Transform
			Vector3 scaling;
			Vector3 rotation;
			Vector3 translation;

			//コライダー
			Collider* collider;
			std::string colliderType;
			Vector3 center;
			Vector3 size;
		};

		//ハンドル
		uint32_t handle = 0u;

		//オブジェクト
		std::vector<ObjectData> objects = {};

		//フォルダ名
		std::string folderName = {};
		//ファイル名
		std::string fileName = {};
		//フルパス
		std::string fullPath = {};

	};



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
	std::map<std::string , std::unique_ptr<LevelData>> levelDatas_;

	uint32_t handle_ = 0u;


};

