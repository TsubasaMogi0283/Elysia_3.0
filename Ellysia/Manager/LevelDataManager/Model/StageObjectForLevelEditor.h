#pragma once
#include "IObjectForLevelEditor.h"
#include "StageObjectForLevelEditorCollider.h"


class StageObjectForLevelEditor :public IObjectForLevelEditor {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageObjectForLevelEditor() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelhandle"></param>
	/// <param name="transform"></param>
	void Initialize(const uint32_t& modelhandle, const Transform& transform) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;


#pragma region 描画

	/// <summary>
	/// 描画(平行光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(const Camera& camera,const Material& material,const DirectionalLight& directionalLight)override;

	/// <summary>
	/// 描画(点光源)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="pointLight"></param>
	void Draw(const Camera& camera,const Material& material,const PointLight& pointLight);

	/// <summary>
	/// 描画(スポットライト)
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(const Camera& camera,const Material& material,const SpotLight& spotLight);



private:


	//当たり判定
	std::unique_ptr<StageObjectForLevelEditorCollider>stageObjectForLevelEditorCollider_ = nullptr;


};

