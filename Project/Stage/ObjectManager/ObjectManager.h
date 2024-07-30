#pragma once
#include "Model.h"

#include "WorldTransform.h"
#include "Material.h"

//LevelEditorでステージを作るつもり

struct Camera;
struct SpotLight;

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
	~ObjectManager() = default;

private:
	//各オブジェクトをリストにするかも
	WorldTransform worldTransform_ = {};
	Material material_ = {};
	std::unique_ptr<Model>model_ = nullptr;

};

