#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

/// <summary>
/// オブジェクトの種類
/// </summary>
enum LevelEditorObjectType {
	StageObject,
	AudioObject,
};


/// <summary>
/// レベルエディタ用のモデル(基底クラス)
/// </summary>
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
	/// 平行光源
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	virtual void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight)=0;

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="pointLight"></param>
	virtual void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight) = 0;

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	virtual void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight)=0;




	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObjectForLevelEditor() = default;

protected:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//オブジェクトのタイプ
	//LevelEditorObjectTypeから選ぶよ
	uint32_t objectType_ = 0u;

};