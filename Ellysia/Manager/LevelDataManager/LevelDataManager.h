#pragma once
#include <string>
#include <list>
#include <map>
#include <memory>
#include <fstream>

#include <Vector3.h>
#include <json.hpp>
#include <Model.h>
#include "WorldTransform.h"
#include "Collider/Collider.h"
#include <Transform.h>
#include <Model/IObjectForLevelEditor.h>
#include <Model/AudioObjectForLevelEditor.h>


struct Camera;
struct Material;
struct DirectionalLight;
struct PointLight;
struct SpotLight;

/// <summary>
/// リスナー
/// </summary>
struct Listener {
	//位置
	Vector3 position;
	//動き
	Vector3 move;
};

/// <summary>
/// レベルデータ管理クラス
/// </summary>
class LevelDataManager final{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelDataManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelDataManager() = default;

public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static LevelDataManager* GetInstance();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="levelDataManager"></param>
	LevelDataManager(const LevelDataManager& levelDataManager) = delete;

	/// <summary>
	/// 代入演算子を無効にする
	/// </summary>
	/// <param name="levelDataManager"></param>
	/// <returns></returns>
	LevelDataManager& operator=(const LevelDataManager& levelDataManager) = delete;

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
	void Reload(const uint32_t& levelDataHandle);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="levelDataHandle"></param>
	void Update(const uint32_t&levelDataHandle);

	/// <summary>
	/// 消去
	/// </summary>
	/// <param name="levelDataHandle"></param>
	void Delete(const uint32_t& levelDataHandle);


#pragma region 描画

	/// <summary>
	/// 描画(平行光源)
	/// </summary>
	/// <param name="levelDataHandle"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	void Draw(const uint32_t& levelDataHandle,const Camera& camera, const Material& material, const DirectionalLight& directionalLight);

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="levelDataHandle"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="pointLight"></param>
	void Draw(const uint32_t& levelDataHandle,const Camera& camera, const Material& material, const PointLight& pointLight);

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="levelDataHandle"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	void Draw(const uint32_t& levelDataHandle,const Camera& camera,const Material & material,const SpotLight& spotLight);


#pragma endregion

	/// <summary>
	/// 解放
	/// デストラクタの代わり
	/// </summary>
	void Release();


private:

	/// <summary>
	/// レベルデータ
	/// </summary>
	struct LevelData {
		//モデルオブジェクト
		struct ObjectData {
			//オブジェクトのタイプ
			//今はステージかオーディオのどちらか
			std::string type;

			//ファイル名
			std::string modelFileName;
			
			//Transform
			Transform transform;

			
#pragma region コライダー
			//コライダーを持っているかどうか
			bool isHavingCollider=false;

			//Colliderの種類
			std::string colliderType;
			//Sphere,Box
			Vector3 center;
			Vector3 size;

			//AABB
			AABB aabb;
			Vector3 upSize;
			Vector3 downSize;
#pragma endregion

			//レベルデータのオーディオ
			AudioDataForLevelEditor levelAudioData;

			//オブジェクト(ステージかオーディオ)
			IObjectForLevelEditor* objectForLeveEditor;

			//コライダー
			IObjectForLevelEditorCollider* levelDataObjectCollider;


		};



		//ハンドル
		uint32_t modelHandle;

		//オブジェクトのリスト
		std::list<ObjectData> objectDatas;


		//リスナー
		//プレイヤーなどを設定してね
		Listener listener;

		//フォルダ名
		std::string folderName;
		//ファイル名
		std::string fileName;
		//フルパス
		std::string fullPath;

	};



	/// <summary>
	/// 指定したオブジェクトの取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::list<LevelData::ObjectData> GetObject(const uint32_t& handle) {
		
		for (const auto& [key, levelData] : levelDatas_) {
			
			//一致したら返す
			if (levelData->modelHandle == handle) {
				return levelData->objectDatas;
				
			}
		}

		//見つからない
		return {};
	}

public:
	/// <summary>
	/// リスナーの設定
	/// </summary>
	/// <param name="handle"></param>
	/// <param name="listener"></param>
	inline void SetListener(const uint32_t& handle, const Listener& listener) {
		for (const auto& [key, levelData] : levelDatas_) {

			//一致したら設定
			if (levelData->modelHandle == handle) {
				levelData->listener = listener;
			}
		}
	}


private:
	/// <summary>
	/// 配置
	/// </summary>
	void Place(nlohmann::json& objects, LevelData& levelData);

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="directoryPath"></param>
	void Ganarate(LevelData& levelData);

	
	/// <summary>
	/// JSONファイルを解凍
	/// </summary>
	/// <param name="fullFilePath"></param>
	/// <returns></returns>
	nlohmann::json Deserialize(const std::string& fullFilePath);

	/// <summary>
	/// 拡張子を取得
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="baseFileName"></param>
	/// <returns></returns>
	std::string FindExtension(const std::string& directory, const std::string& baseFileName);
private:

	//ここにデータを入れていく
	std::map<std::string , std::unique_ptr<LevelData>> levelDatas_;

	//ハンドル
	uint32_t handle_ = 0u;

	//Resourceにあるレベルデータの場所
	const std::string leveldataPath_ = "Resources/LevelData/";

};

