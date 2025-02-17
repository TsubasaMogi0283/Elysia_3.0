#pragma once
/**
 * @file KeyManager.h
 * @brief 鍵のクラス
 * @author 茂木翼
 */

#include <memory>
#include <list>
#include <sstream>

#include "Audio.h"
#include "Key.h"
#include "Sprite.h"


#pragma region 前方宣言

/// <summary>
/// カメラ
/// </summary>
struct Camera;

/// <summary>
/// スポットライト
/// </summary>
struct SpotLight;

/// <summary>
/// プレイヤー
/// </summary>
class Player;


/// <summary>
/// EllysiaEngine(前方宣言)
/// </summary>
namespace Ellysia{
	/// <summary>
	/// オーディオ
	/// </summary>
	class Audio;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager;

	/// <summary>
	/// 入力
	/// </summary>
	class Input;

}


#pragma endregion


/// <summary>
/// 鍵管理クラス
/// </summary>
class KeyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="csvPath"></param>
	void Initialize(const uint32_t& modelHandle,const std::string& csvPath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void DrawObject3D(const Camera& camera,const SpotLight& spotLight);
	
	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyManager()=default;


private:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="position"></param>
	void Genarate(const Vector3& position);

	/// <summary>
	/// 消去
	/// </summary>
	void Delete();

	/// <summary>
	/// 取得
	/// </summary>
	void PickUp();

	

public:
	/// <summary>
	/// 鍵のリストを取得
	/// </summary>
	/// <returns></returns>
	inline std::list<Key*> GetKeyes() const{
		std::list <Key*> keys = {};
		for (const std::unique_ptr<Key>& key : keies_) {
			keys.push_back(key.get());
		}

		return keys;
	}

	/// <summary>
	/// 今ステージ上にある鍵の数を取得
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetKeyQuantity() const {
		return static_cast<uint32_t>(keies_.size());
	}

	/// <summary>
	/// 最大で取得できる数数
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetMaxKeyQuantity()const {
		return MAX_KEY_QUANTITY_;
	}


public:
	/// <summary>
	/// プレイヤー
	/// </summary>
	/// <param name="player"></param>
	/// <returns></returns>
	inline void SetPlayer(Player* player) { 
		this->player_ = player;
	};

	/// <summary>
	/// 音が止まる
	/// </summary>
	void StopAudio();

private:
	//オーディオ
	Ellysia::Audio* audio_ = nullptr;
	//テクスチャ管理クラス
	Ellysia::TextureManager* textureManager_ = nullptr;
	//入力
	Ellysia::Input* input_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;


private:
	//最大数
	static const uint32_t MAX_KEY_QUANTITY_ = 3u;
	//数
	static const uint32_t NUMBER_QUANTITY_ = 10u;
	//鍵の音が聞こえる最大距離
	const float MAX_DISTANCE_ = 30.0f;
	//トリガーの増える値
	const uint32_t INCREASE_VALUE = 1u;
	//反応する
	const uint32_t B_REACT_TIME_ = 1u;
	//反応しない
	const uint32_t B_NO_REACT_TIME_ = 0u;
	//コントローラーの押していない時の値
	const int32_t NO_PUSH_VALUE_ = 0u;

	
private:
	//鍵
	std::list<std::unique_ptr<Key>>keies_ = {};
	//モデルハンドル
	uint32_t modelHandle_ = 0u;

	//UI
	std::unique_ptr<Ellysia::Sprite> keyListSprite_ = nullptr;
	//鍵
	std::array<std::unique_ptr<Ellysia::Sprite>, MAX_KEY_QUANTITY_> keySprites_ = { nullptr };
	//サイズ
	uint64_t keySpriteWidth_ = 0u;
	uint64_t keySpriteHeight_ = 0u;

	//スケール
	std::array<float, MAX_KEY_QUANTITY_> spriteTs_ = {};

	//初期座標
	Vector2 initialPosition_ = { .x = 20.0f,.y = 10.0f };
	Vector2 initialPositionAddAnchorPoint = {};

	//終点座標
	std::array<Vector2, MAX_KEY_QUANTITY_> endPositions_ = {};
	//鍵取得するかどうか
	std::unique_ptr<Ellysia::Sprite> pickUpKey_ = nullptr;
	//プレイヤーと全ての鍵の距離
	std::list<float>keyAndPlayerDistances_ = {};
	
	//生成の文字列を入れる
	std::stringstream keyPositionsFromCSV_ = {};


	//拾う音
	uint32_t pickUpSEHandle = 0u;
	//鍵の場所を知らせる音
	uint32_t notificationSEHandle_ = 0u;
	//取得可能か
	bool isAbleToPickUpKey_ = false;
	//鍵の数
	uint32_t keyQuantity_ = 0u;



	//Bボタンのトリガー
	uint32_t bTriggerTime_ = 0u;
	bool isBTrigger_ = false;



};

