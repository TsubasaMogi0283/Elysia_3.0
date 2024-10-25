#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Collider/Collider.h"


//レベルエディタ用のモデル
class IObjectForLevelEditor {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const uint32_t& modelhandle,const Vector3& position)=0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObjectForLevelEditor() = default;

protected:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//コライダー
	std::unique_ptr<Collider> collider_ = nullptr;

};