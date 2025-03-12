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




/// <summary>
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Ellysia {
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
class LoseScene : public Ellysia::IGameScene {
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
	void Update(Ellysia::GameManager* gameManager)override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;

	/// <summary>
	/// ポストエフェクト描画処理前
	/// </summary>
	void PreDrawPostEffectFirst()override;

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

private:
	/// <summary>
	/// 選択
	/// </summary>
	void Select();

	/// <summary>
	/// 次のシーンへ変わる
	/// </summary>
	void ChangeNextScene();

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
	Ellysia::Input* input_ = nullptr;
	//テクスチャ管理クラス
	Ellysia::TextureManager* textureManager_ = nullptr;
	//レベルデータ管理クラス
	Ellysia::LevelDataManager* levelDataManager_ = nullptr;
	//ハンドル
	uint32_t levelDataHandle_ = 0u;

	//モデル管理クラス
	Ellysia::ModelManager* modelManager_ = nullptr;
	//グローバル変数クラス
	Ellysia::GlobalVariables* globalVariables_ = nullptr;
	
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
	//速度
	Vector3 cameraVelocity_ = {};

	//点光源
	PointLight pointLight_ = {};
	//半径の線形補間
	float startLightUpT_ = 0.0f;
	float endLightUpT_ = 0.0f;

	//背景(ポストエフェクト)
	std::unique_ptr<Ellysia::BackTexture>backTexture_ = nullptr;
	//ディゾルブ
	std::unique_ptr<Ellysia::DissolvePostEffect> dissolveEffect_ = nullptr;
	Dissolve dissolve_ = {};
	//黒背景
	std::unique_ptr<Ellysia::Sprite> black_ = nullptr;
	//透明度
	float transparency_ = 0.0f;
	
	//暗転している時間
	uint32_t blackOutTime_ = 0u;

	//タイトル
	bool isChangeNextScene_ = false;
	float returnToTitleDissolveThresholdT_ = 0.0f;
	
	//選択中
	//タイトル
	bool isSelectingTitle_ = false;
	//ゲーム
	bool isSelectingGame_ = true;
	
	//選択時のスケール
	float selectedScale_ = 1.5f;

	//テキストの透明度
	float textTransparency_ = 0.0f;
	float transparencyT_ = 0.0f;
	bool displayText_ = false;
	
	//矢印の回転
	float arrowRotate_ = 0.0f;
	//決定したときの線形補間
	float arrowDropT_ = 0.0f;

	//決定時の動きに使うθ
	float decideArrowMoveTheta_ = 0.0f;

	//高速点滅
	bool isFastFlash_ = false;
	//時間
	uint32_t waitForCameraMoveTime_ = 0u;

	
	//ライトアップが終わったかどうか
	bool isFinishLightUp_ = false;


};

