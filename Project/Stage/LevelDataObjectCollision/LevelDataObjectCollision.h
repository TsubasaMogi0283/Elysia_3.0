#pragma once
#include "Collider/Collider.h"

class LevelDataObjectCollision : public Collider{
public:


	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelDataObjectCollision() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialze(std::string& type,Vector3& position,Vector3& size);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="position"></param>
	void Update(Vector3& position);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelDataObjectCollision() = default;

private:



};

