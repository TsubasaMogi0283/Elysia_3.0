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
	void Initialize(const uint32_t& modelhandle, const Vector3& position) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;


#pragma region 描画

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="pointLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(WorldTransform & worldTransform, Camera& camera, Material& material, SpotLight& spotLight);



private:


	//当たり判定
	std::unique_ptr<StageObjectForLevelEditorCollider>stageObjectForLevelEditorCollider_ = nullptr;


};

