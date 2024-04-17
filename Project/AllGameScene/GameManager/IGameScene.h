#pragma once

//前方宣言
class GameManager;

class IGameScene {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IGameScene() {};
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()=0;
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gamaManager"></param>
	virtual void Update(GameManager* gamaManager)=0;
	
	/// <summary>
	/// Object3Dの描画
	/// </summary>
	virtual void DrawObject3D()=0;

	/// <summary>
	/// スプライトの描画
	/// </summary>
	virtual void DrawSprite() = 0;

};