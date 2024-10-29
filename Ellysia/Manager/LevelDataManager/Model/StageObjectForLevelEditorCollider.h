#pragma once
#include "IObjectForLevelEditorCollider.h"

class StageObjectForLevelEditorCollider :public IObjectForLevelEditorCollider {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;


	

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision()override;


	/// <summary>
	/// 比衝突
	/// </summary>
	void OffCollision()override;


	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;





private:





};

