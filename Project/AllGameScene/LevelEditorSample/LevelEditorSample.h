#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "Material.h"
#include "BackText.h"
#include "LevelDataManager.h"
#include "Collider/CollisionManager.h"
#include "Vignette.h"
#include "Particle3D.h"

#include "Audio.h"
#include "Player/AudioTestPlayer.h"


class GameManager;

/// <summary>
/// レベルデータの動作確認クラス
/// </summary>
class LevelEditorSample : public IGameScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelEditorSample();


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
	void Update(GameManager* gameManager)override;

#pragma region 描画

	/// <summary>
	/// ポストエフェクト掛ける前のスプライト
	/// </summary>
	void DrawSpriteBack()override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画前処理
	/// </summary>
	void PreDrawPostEffectFirst()override;
	
	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelEditorSample();




private:
	//ビネット
	std::unique_ptr<BackText> back_ = nullptr;
	//カメラ
	Camera camera_ = {};
	//マテリアル
	Material material_ = {};
	//平行光源
	DirectionalLight directionalLight_ = {};
	//インプット
	Input* input_ = nullptr;


	//レベルエディタ
	LevelDataManager* levelEditor_ = nullptr;
	uint32_t levelHandle_ = 0u;


	//サンプル
	uint32_t audioHandle_ = 0u; 
	uint32_t audioHandle2_ = 0u;
	uint32_t audioHandleMP3_ = 0u;

	//プレイヤー
	std::unique_ptr<AudioTestPlayer>player_ = nullptr;
	Vector3 playerDirection_ = {};

	//パーティクル
	std::unique_ptr<Particle3D>particle3D_ = nullptr;


	//コリジョンマネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;


};

