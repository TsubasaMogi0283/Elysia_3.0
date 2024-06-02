#pragma once

#include "IGameScene.h"


//main.cppにあるものを全部こっちに引っ越しする
class GameManager {
public:
	//コンストラクタ
	GameManager();

	//初期化
	void Initialize();

	//シーンチェンジ
	void ChangeScene(IGameScene* newGameScene);

	//更新
	void Update();

	//描画
	void DrawSpriteBack();
	void DrawObject3D();
	void DrawSprite();

	//PostEffect
	void PreDrawPostEffectFirst();
	void PreDrawPostEffectSecond();
	void DrawPostEffect();
	



	//デストラクタ
	~GameManager();

private:

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;



	
};