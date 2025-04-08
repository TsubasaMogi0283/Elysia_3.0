#pragma once
/**
 * @file LoseScene.h
 * @brief 負けシーンのクラス
 * @author 茂木翼
 */
#include <memory>

#include "IGameScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Camera.h"
#include "PointLight.h" 
#include "BackTexture.h"
#include "DissolveEffect.h"
#include "Dissolve.h"
#include "BaseLoseScene/BaseLoseScene.h"




/// <summary>
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Elysia {
	/// <summary>
	/// 入力
	/// </summary>
	class Input;

	/// <summary>
	/// レベルデータ管理クラス
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// グローバル変数
	/// </summary>
	class GlobalVariables;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameManager;


	/// <summary>
	/// モデル管理クラス
	/// </summary>
	class ModelManager;


};


/// <summary>
/// 負けシーン
/// </summary>
class LoseScene : public Elysia::IGameScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoseScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager"></param>
	void Update(Elysia::GameManager* gameManager)override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffect()override;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト描画
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LoseScene()=default;

public:
	/// <summary>
	/// 敗北の細かいシーンの遷移
	/// </summary>
	/// <param name=""></param>
	void ChangeDetailScene(std::unique_ptr<BaseLoseScene> detailScene);

	/// <summary>
	/// 終わったことを設定する
	/// </summary>
	inline void SetIsEnd() {
		this->isEnd_ = true;
	}

	/// <summary>
	/// ゲームを続けるかどうかの設定
	/// </summary>
	/// <param name="isContinue">フラグ</param>
	inline void SetIsContinue(const bool& isContinue) {
		this->isContinue_ = isContinue;
	}

private:
	/// <summary>
	/// /ImGUiの表示
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 調整
	/// </summary>
	void Adjustment();

private:
	//入力クラス
	Elysia::Input* input_ = nullptr;
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//レベルデータ管理クラス
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelDataHandle_ = 0u;

	//モデル管理クラス
	Elysia::ModelManager* modelManager_ = nullptr;
	//グローバル変数クラス
	Elysia::GlobalVariables* globalVariables_ = nullptr;
	
private:
	//調整項目
	//点光源
	const std::string POINT_LIGHT_NAME_ = "LoseScenePointLight";
	//ディゾルブ
	const std::string DISSOLVE_NAME_ = "LoseSceneDissolve";

	//時間変化
	const float DELTA_TIME = 1.0f / 60.0f;

	//間隔
	const float TRANSPARENCY_INTERVAL_ = 0.01f;
	//点滅どのくらい
	const uint32_t FLASH_TIME_LIMIT_ = 30u;

	
	//高速点滅間隔
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3u;
	//タイトルに変わる時間
	const uint32_t CHANGE_TO_TITLE_TIME_ = 60 * 1;
	//増える時間の値
	const uint32_t INCREASE_VALUE_ = 1u;
	//線形補間で使う変数の最大値
	const float MAX_T_VALUE_ = 1.0f;
	//最大の半径
	const float MAX_LIGHT_RADIUS_ = 11.8f;
	//矢印の回転
	const float ROTATE_VALUE_ = 0.1f;

	//テキストの名前
	//矢印
	const std::string SELECT_ARROW = "SelectArrow";
	//ゲーム
	const std::string TO_GAME = "ToGame";
	//タイトル
	const std::string TO_TITLE = "ToTitle";
	
private:
	//カメラ
	Camera camera_ = {};

	//点光源
	PointLight pointLight_ = {};

	//背景(ポストエフェクト)
	std::unique_ptr<Elysia::BackTexture>backTexture_ = nullptr;
	//ディゾルブ
	std::unique_ptr<Elysia::DissolvePostEffect> dissolveEffect_ = nullptr;
	Dissolve dissolve_ = {};
	//黒背景
	std::unique_ptr<Elysia::Sprite> black_ = nullptr;
	

	//細かいシーン
	std::unique_ptr<BaseLoseScene> detailLoseScene_ = nullptr;
	//処理が終わったかどうか
	bool isEnd_ = false;
	//ゲームを続けるかどうか
	bool isContinue_ = false;


};

