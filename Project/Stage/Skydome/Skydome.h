#pragma once
#include <WorldTransform.h>
#include <Material.h>
#include <DirectionalLight.h>
#include <Model.h>

struct Camera;

class Skydome{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skydome() = default;


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	void Initialize(uint32_t modelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera& camera);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Skydome() = default;


private:
	std::unique_ptr<Model>model_ = nullptr;


	WorldTransform worldTransform_ = {};
	Material material_ = {};
	DirectionalLight directionalLight_ = {};

};

