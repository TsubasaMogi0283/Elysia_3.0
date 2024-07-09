#pragma once

struct Camera;
struct SpotLight;

class Level1{


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Level1() = default;

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
	~Level1() = default;

private:



};

