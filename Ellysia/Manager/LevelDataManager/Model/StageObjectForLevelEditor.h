#pragma once
#include "IObjectForLevelEditor.h"


class StageObjectForLevelEditor :public IObjectForLevelEditor {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const uint32_t& modelhandle, const Vector3& position) override;


#pragma region 描画

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="camera">カメラ</param>
	/// <param name="directionalLight">平行光源</param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, DirectionalLight& directionalLight);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="pointLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, PointLight& pointLight);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, Material& material, SpotLight& spotLight);



private:



};

