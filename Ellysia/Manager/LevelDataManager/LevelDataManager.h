#pragma once
#include <string>
#include <list>
#include <array>
#include <map>
#include <memory>
#include <fstream>

#include <Vector3.h>
#include <json.hpp>
#include <Model.h>
#include "WorldTransform.h"
#include "Collider/Collider.h"
#include <Transform.h>
#include <Model/IObjectForLevelEditorCollider.h>
#include <Model/IObjectForLevelEditor.h>


struct Camera;
struct Material;
struct DirectionalLight;
struct PointLight;
struct SpotLight;

struct Listener {
	//位置
	Vector3 position;
	//動き
	Vector3 move;
};

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
	//シングルインスタンス
	static LevelDataManager* GetInstance();

	//コピーコンストラクタ禁止
	LevelDataManager(const LevelDataManager& levelDataManager) = delete;

	//代入演算子を無効にする
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
	void Reload(uint32_t& levelDataHandle);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="levelDataHandle"></param>
	void Update(uint32_t&levelDataHandle);

	/// <summary>
	/// 消去
	/// </summary>
	/// <param name="levelDataHandle"></param>
	void Delete(uint32_t& levelDataHandle);


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
	/// デストラクタの代わり
	/// </summary>
	void Release();


private:
	

	struct LevelDataAudioData {
		//オーディオデータを持っているかどうか
		bool isHavingAudio;

		//ファイル名
		std::string fileName;

		//種類
		std::string type;

		//ハンドル
		uint32_t handle;

		//エリア上かどうか
		bool isOnArea;

		//ループ
		bool isLoop;
	};

	struct LevelData {
		//モデルオブジェクト
		struct ObjectData {
			//モデル
			Model* model = nullptr;
			//ワールドトランスフォーム
			WorldTransform* worldTransform = {};

			//ファイル名
			std::string modelFileName;
			
			//Transform
			Transform transform;

			//Colliderの種類
#pragma region コライダー
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
			LevelDataAudioData levelAudioData;

			//オブジェクト(ステージかオーディオ)
			IObjectForLevelEditor* object_;

			//当たり判定(ステージかオーディオ)
			IObjectForLevelEditorCollider* objectCollider_;


		};




		//リスナー(通常はプレイヤーを入れる)
		Listener listener_ = {};

		//ハンドル
		uint32_t handle = 0u;

		//オブジェクト
		std::list<ObjectData> objectDatas = {};



		//フォルダ名
		std::string folderName = {};
		//ファイル名
		std::string fileName = {};
		//フルパス
		std::string fullPath = {};

	};



	/// <summary>
	/// 指定したオブジェクトの取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::list<LevelData::ObjectData> GetObject(uint32_t& handle) {
		
		for (const auto& [key, levelData] : levelDatas_) {
			
			//一致したら返す
			if (levelData->handle == handle) {
				return levelData->objectDatas;
			}
		}

		//見つからない
		return {};
	}

	/// <summary>
	/// リスナーの設定
	/// </summary>
	/// <param name="handle"></param>
	/// <param name="listener"></param>
	inline void SetListener(uint32_t& handle,Listener &listener) {

		for (const auto& [key, levelData] : levelDatas_) {
			//一致したら値を代入
			if (levelData->handle == handle) {
				levelData->listener_ = listener;
				break;
			}
		}


	}

private:
	/// <summary>
	/// 配置
	/// </summary>
	void Place(nlohmann::json& objects, LevelData& levelData);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	void Ganarate(LevelData& levelData);

	
	/// <summary>
	/// JSONファイルを解凍
	/// </summary>
	/// <param name="fullFilePath"></param>
	/// <returns></returns>
	nlohmann::json Deserialize(std::string& fullFilePath);

	/// <summary>
	/// オーディオの再生
	/// </summary>
	void AudioPlay(LevelData& levelData);

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

