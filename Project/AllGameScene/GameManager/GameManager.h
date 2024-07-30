#pragma once

#include "IGameScene.h"


//main.cppにあるものを全部こっちに引っ越しする
class GameManager {
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// シーンチェンジ
	/// </summary>
	/// <param name="newGameScene"></param>
	void ChangeScene(IGameScene* newGameScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawSpriteBack();
	void DrawObject3D();
	void DrawSprite();

	/// <summary>
	/// ポストエフェクト
	/// </summary>
	void PreDrawPostEffectFirst();
	void DrawPostEffect();
	



	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

private:

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;



	
};