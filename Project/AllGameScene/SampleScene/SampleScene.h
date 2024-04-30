#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

#include <memory>
#include <Particle3D.h>
#include <Audio.h>

struct LevelData {
	struct ObjectData {
		//ファイル名
		std::string fileName;
		//Transform
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

	};

	std::vector<ObjectData> objects;
	
};


//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;



private:

	float cutOff_ = 1.0f;

	std::unique_ptr<Model> model_ = nullptr;
	uint32_t modelHandle = 0;
	WorldTransform worldTransform_ = {};
	Camera camera_ = {};

	Skeleton skeleton_ = {};
	float animationTime_ = 0;


	const char* GroupName = "Player";
};