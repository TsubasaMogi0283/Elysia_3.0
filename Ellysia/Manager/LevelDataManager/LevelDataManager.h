#pragma once

/**
 * @file LevelDataManager.h
 * @brief レベルデータ管理クラス
 * @author 茂木翼
 */

#include <string>
#include <list>
#include <map>
#include <memory>
#include <fstream>
#include <json.hpp>

#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"
#include "Transform.h"
#include "Model/IObjectForLevelEditor.h"
#include "Model/AudioObjectForLevelEditor.h"

#pragma region 前方宣言

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// マテリアル
/// </summary>
struct Material;

/// <summary>
/// 平行光源
/// </summary>
struct DirectionalLight;

/// <summary>
/// 点光源
/// </summary>
struct PointLight;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

#pragma endregion

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


public:

	/// <summary>
	/// データにある分だけColliderを取得する
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::vector<IObjectForLevelEditorCollider*> GetCollider(const uint32_t& handle) {
		std::vector<IObjectForLevelEditorCollider*> colliders = {};

		for (const auto& [key, levelData] : levelDatas_) {
			if (levelData->handle == handle) {

				//該当するLevelDataのobjectDatasを検索
				for (auto& objectData : levelData->objectDatas) {

					//コライダーを持っている場合、リストに追加
					if (objectData.levelDataObjectCollider != nullptr) {
						colliders.push_back(objectData.levelDataObjectCollider);
					}
				}

				//無駄なループを防ぐ
				break;
			}
		}

		return colliders;
	}

	/// <summary>
	/// オーディオの方のコライダーを取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::vector<IObjectForLevelEditorCollider*>GetAudioCollider(const uint32_t& handle) {
		std::vector<IObjectForLevelEditorCollider*> colliders = {};

		for (const auto& [key, levelData] : levelDatas_) {
			if (levelData->handle == handle) {

				//該当するLevelDataのobjectDatasを検索
				for (auto& objectData : levelData->objectDatas) {

					//コライダーを持っている場合、リストに追加
					if (objectData.levelDataObjectCollider != nullptr && objectData.type == "Audio") {
						colliders.push_back(objectData.levelDataObjectCollider);
					}
				}

				//無駄なループを防ぐ
				break;
			}
		}

		return colliders;
	}


	/// <summary>
	/// ステージオブジェクトの座標を取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::vector<Vector3> GetStageObjectPositions(const uint32_t& handle) {
		std::vector<Vector3> positions = {};

		for (const auto& [key, levelData] : levelDatas_) {
			if (levelData->handle == handle) {


				//該当するLevelDataのobjectDatasを検索
				for (auto& objectData : levelData->objectDatas) {

					//Stageだったら追加
					if (objectData.type == "Stage") {
						positions.push_back(objectData.objectForLeveEditor->GetWorldPosition());
					}


				}

				//無駄なループを防ぐ
				break;
			}
		}

		return positions;
	}
	
	/// <summary>
	/// ステージオブジェクトのAABBを取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::vector<AABB> GetStageObjectAABBs(const uint32_t& handle) {
		std::vector<AABB> aabbs = {};

		for (const auto& [key, levelData] : levelDatas_) {
			if (levelData->handle == handle) {


				//該当するLevelDataのobjectDatasを検索
				for (auto& objectData : levelData->objectDatas) {

					//Stageだったら追加
					if (objectData.type == "Stage") {
						aabbs.push_back(objectData.objectForLeveEditor->GetAABB());
					}


				}

				//無駄なループを防ぐ
				break;
			}
		}

		return aabbs;
	}

	/// <summary>
	/// コライダーを持っているかどうかの取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	inline std::vector<bool> GetIsHavingColliders(const uint32_t& handle) {
		std::vector<bool> colliders = {};

		for (const auto& [key, levelData] : levelDatas_) {
			if (levelData->handle == handle) {


				//該当するLevelDataのobjectDatasを検索
				for (auto& objectData : levelData->objectDatas) {

					//コライダーを持っているかどうかのフラグを挿入
					colliders.push_back(objectData.isHavingCollider);

				}

				//無駄なループを防ぐ
				break;
			}
		}

		return colliders;
	}

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

			uint32_t colliderTypeNumber;


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
		uint32_t handle = 0u;

		//オブジェクトのリスト
		std::list<ObjectData> objectDatas;

		//リスナー
		//プレイヤーなどを設定してね
		Listener listener = {};

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
			if (levelData->handle == handle) {
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
			if (levelData->handle == handle) {
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
	const std::string LEVEL_DATA_PATH_ = "Resources/LevelData/";

};

