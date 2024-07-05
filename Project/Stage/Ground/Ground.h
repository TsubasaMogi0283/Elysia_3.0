#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "StageRect.h"
struct Camera;
struct SpotLight;

class Ground{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Ground() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelhandle"></param>
	void Initialize(uint32_t modelhandle);

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
	~Ground() = default;


public:
	/// <summary>
	/// 四隅の座標を取得
	/// </summary>
	/// <returns></returns>
	inline StageRect GetStageRect() const {
		return stageRect_;
	}


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};

	//ステージスケールの大きさ
	const float SCALE_SIZE_ = 40.0f;
	StageRect stageRect_ = {};

};

