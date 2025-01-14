#pragma once

/**
 * @file AudioObjectForLevelEditor.h
 * @brief オーディオ用のオブジェクトクラス
 * @author 茂木翼
 */



#include <memory>
#include <cstdint>

#include "IObjectForLevelEditor.h"
#include "AudioObjectForLevelEditorCollider.h"
#include "Audio.h"

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
/// オーディオオブジェクトのタイプ
/// </summary>
enum AudioObjectType {
	BGMType,
	ActionType,
};

/// <summary>
/// オーディオオブジェクトのデータ
/// </summary>
struct AudioDataForLevelEditor {

	//ファイル名
	std::string fileName;

	//種類(BGMかSE)
	std::string type;

	//ハンドル
	uint32_t handle;

	//エリア上かどうか
	bool isOnArea;

	//ループ
	bool isLoop;
};

/// <summary>
/// オーディオ用のオブジェクト
/// </summary>
class AudioObjectForLevelEditor :public IObjectForLevelEditor {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioObjectForLevelEditor()=default;


	/// <summary>
	/// オーディオのハンドルを設定
	/// 初期化より先でやってね
	/// </summary>
	inline void SetLevelDataAudioData(const AudioDataForLevelEditor& levelDataAudioData) {
		this->audioDataForLevelEditor_ = levelDataAudioData;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelhandle"></param>
	/// <param name="transform"></param>
	void Initialize(const uint32_t& modelhandle, const Transform& transform) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	

#pragma region 描画

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="pointLight"></param>
	void Draw(const Camera& camera,const Material& material,const PointLight& pointLight)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const Material& material,const SpotLight& spotLight)override;




#pragma endregion
public:
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// AABBの取得
	/// </summary>
	/// <returns></returns>
	AABB GetAABB()override;

private:
	/// <summary>
	/// BGM
	/// </summary>
	void BackGround();

	/// <summary>
	/// アクションSE
	/// </summary>
	void Action();

	
	
private:
	//オーディオ
	Ellysia::Audio* audio_ = nullptr;
	
	//レベルエディタ側で設定した値を持ってくる
	AudioDataForLevelEditor audioDataForLevelEditor_ = {};
	
	int32_t audioType_ = -1;

};