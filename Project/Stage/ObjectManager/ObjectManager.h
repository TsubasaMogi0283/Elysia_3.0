#pragma once

/**
 * @file ObjectManager.h
 * @brief オブジェクト管理クラス
 * @author 茂木翼
 */

#include <list>
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

//LevelEditorでステージを作るつもり

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

/// <summary>
/// オブジェクト管理クラス
/// </summary>
class ObjectManager{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjectManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(Camera& camera,SpotLight& spotLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ObjectManager();


private:
	//各オブジェクトをリストにするかも
	Material material_ = {};
	

};

