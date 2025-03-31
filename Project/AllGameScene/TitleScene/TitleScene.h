#pragma once
/**
 * @file TitleScene.h
 * @brief タイトルクラス
 * @author 茂木翼
 */

#include <memory>
#include <vector>

#include "IGameScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Camera.h"
#include "BackTexture.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "RailCamera/TitleRailCamera.h"
#include "RandomEffect.h"
#include "BaseBackTexture/BaseTitleBackTexture.h"

#pragma region 前方宣言


 /// <summary>
 /// EllysiaEngine
 /// </summary>
namespace Elysia {
	/// <summary>
	/// 入力
	/// </summary>
	class Input;

	/// <summary>
	/// レベル管理クラス
	/// </summary>
	class LevelDataManager;

	/// <summary>
	/// ゲーム管理クラス
	/// </summary>
	class GameManager;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;


}


#pragma endregion


/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Elysia::IGameScene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameManager">ゲーム管理クラス</param>
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
	/// ポストエフェクト描画
	/// </summary>
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene() = default;



private:
	/// <summary>
	/// ImGui用関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 背景テクスチャの遷移
	/// </summary>
	/// <param name="backTexture">背景テクスチャ(ポストエフェクト)</param>
	void ChangeBackTexture(std::unique_ptr<BaseTitleBackTexture>);

private:
	//テクスチャ管理クラス
	Elysia::TextureManager* textureManager_ = nullptr;
	//入力クラス
	Elysia::Input* input_ = nullptr;
	//レベルエディタ
	Elysia::LevelDataManager* levelDataManager_ = nullptr;
	uint32_t levelHandle_ = 0u;


private:
	//時間の変化
	const float DELTA_TIME_ = 1.0f / 60.0f;

	//ランダムエフェクトの表示時間
	static const uint32_t DISPLAY_LENGTH_QUANTITY_ = 2u;

	//増える時間の値
	const uint32_t INCREASE_VALUE = 1u;

private:

	//カメラ
	Camera camera_ = {};
	std::unique_ptr<TitleRailCamera> titleRailCamera_ = nullptr;

	//座標
	Vector3 cameraPosition_ = {};

	//平行光源
	DirectionalLight directionalLight_ = {};
	//スポットライト
	SpotLight spotLight = {};


	//黒フェード
	std::unique_ptr<Elysia::Sprite>blackFade_ = nullptr;
	//透明度
	float blackFadeTransparency_ = 0.0f;


	//テキスト
	std::unique_ptr<Elysia::Sprite> text_ = nullptr;

	//ロゴ
	std::unique_ptr<Elysia::Sprite> logo = nullptr;
	uint32_t logoTextureHandle_ = 0u;
	//変化後のテクスチャ
	uint32_t changedLogoTextureHandle_ = 0u;


#pragma region ポストエフェクト

	//背景(ポストエフェクト)
	std::unique_ptr<BaseTitleBackTexture> baseTitleBackTexture_ = nullptr;


	//ランダムノイズ
	std::unique_ptr<Elysia::RandomEffect> randomEffect_ = nullptr;
	float randomEffectTime_ = 0u;
	bool isDisplayRandomEffect_ = false;
	bool isEndDisplayRandomEffect_ = false;

#pragma endregion

	//Bボタンのトリガー
	//時間
	uint32_t bTriggerTime_ = 0;
	//フラグ
	bool isBTrigger_ = false;

	//点滅
	bool isFlash_ = false;
	//時間
	uint32_t flashTime_ = 0;
	const uint32_t FLASH_TIME_LIMIT_ = 30;
	//高速点滅
	bool isFastFlash_ = false;
	const uint32_t FAST_FLASH_TIME_LIMIT_ = 60;
	const uint32_t FAST_FLASH_TIME_INTERVAL_ = 3;
	uint32_t fastFlashTime_ = 0;

	//見せる番号
	uint32_t textDisplayCount_ = 0;

	//スタート
	bool isStart_ = false;




};