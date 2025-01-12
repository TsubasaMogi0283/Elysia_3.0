#pragma once
/**
 * @file KeyManager.h
 * @brief 鍵のクラス
 * @author 茂木翼
 */

#include <memory>
#include <list>
#include <sstream>

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
/// オーディオ
/// </summary>
class Audio;

/// <summary>
/// テクスチャ管理クラス
/// </summary>
class TextureManager;

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
	/// <param name="playeresKey"></param>
	void DrawSprite(const uint32_t& playeresKey);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyManager();


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


public:
	/// <summary>
	/// 鍵のリストを取得
	/// </summary>
	/// <returns></returns>
	inline std::list<Key*> GetKeyes() const{
		return keyes_;
	}

	/// <summary>
	/// 今ステージ上にある鍵の数を取得
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetKeyQuantity() const {
		return static_cast<uint32_t>(keyes_.size());
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
	Audio* audio_ = nullptr;

	//テクスチャ管理クラス
	TextureManager* textureManager_ = nullptr;

	//プレイヤー
	Player* player_ = nullptr;


private:
	//最大数
	const uint32_t MAX_KEY_QUANTITY_ = 3;

	//数
	static const uint32_t NUMBER_QUANTITY_ = 10;

	//鍵の音が聞こえる最大距離
	const float MAX_DISTANCE_ = 30.0f;

private:
	//鍵のリスト
	std::list<Key*>keyes_ = {};

	//UI
	std::unique_ptr<Sprite> keySprite_ = nullptr;
	//スプライト
	std::unique_ptr<Sprite> keyNumber[NUMBER_QUANTITY_] = { nullptr };
	//鍵取得するかどうか
	std::unique_ptr<Sprite> pickUpKey_ = nullptr;

	//プレイヤーと全ての鍵の距離
	std::list<float>keyAndPlayerDistances_ = {};
	
	//モデルハンドル
	uint32_t modelHandle_ = 0u;

	//生成の文字列を入れる
	std::stringstream enemyPositionsFromCSV;


	//拾う音
	uint32_t pickUpSEHandle = 0u;
	//鍵の場所を知らせる音
	uint32_t notificationSEHandle_ = 0u;

};

