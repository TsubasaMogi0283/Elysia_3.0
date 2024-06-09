#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"

struct Camera;

class Enemy{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	void Initialize(uint32_t modelHandle, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy()=default;




private:
	WorldTransform worldTransform_ = {};
	std::unique_ptr<Model> model_ = nullptr;

};

