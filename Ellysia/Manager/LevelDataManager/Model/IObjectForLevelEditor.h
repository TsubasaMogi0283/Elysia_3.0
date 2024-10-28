#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

//種類
enum LevelEditorObjectType {
	StageObject,
	AudioObject,
};


//レベルエディタ用のモデル
class IObjectForLevelEditor {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const uint32_t& modelhandle,const Vector3& position)=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObjectForLevelEditor() = default;

protected:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};


	uint32_t objectType_ = 0u;

};