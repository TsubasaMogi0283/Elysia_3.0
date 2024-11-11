#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include "Material.h"
#include "Transform.h"

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
	/// <param name="modelhandle"></param>
	/// <param name="position"></param>
	virtual void Initialize(const uint32_t& modelhandle,const Transform& position)=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;



	/// <summary>
	/// 平行光源
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	virtual void Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight)=0;

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="pointLight"></param>
	virtual void Draw(const Camera& camera,const Material& material,const PointLight& pointLight) = 0;

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="spotLight"></param>
	virtual void Draw(const Camera& camera,const Material& material,const SpotLight& spotLight)=0;




	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObjectForLevelEditor() = default;


public:
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// 衝突したかどうかの設定
	/// Colliderから持ってくる
	/// </summary>
	/// <param name="isTouch"></param>
	inline void SetIsTouch(const bool& isTouch) {
		this->isTouch_ = isTouch;
	}

	/// <summary>
	/// リスナーが動いているかどうかの設定
	/// </summary>
	/// <param name="isMove"></param>
	/// <returns></returns>
	inline void SetIsListenerMove(const bool& isMove) {
		this->isListenerMove_ = isMove;
	}

protected:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//オブジェクトのタイプ
	//LevelEditorObjectTypeから選ぶよ
	uint32_t objectType_ = 0u;


	//衝突
	bool isTouch_ = false;

	//リスナーが動いているかどうか
	bool isListenerMove_=false;
};