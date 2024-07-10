#pragma once
#include <memory>
#include <list>

#include "Key.h"
#include <TextureManager.h>
#include <Sprite.h>

struct Camera;
struct SpotLight;

class Player;

class KeyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	void Initialize(uint32_t modelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="spotLight"></param>
	void DrawObject3D(Camera& camera,SpotLight& spotLight);
	
	/// <summary>
	/// スプライトの描画
	/// </summary>
	/// <param name="playeresKey"></param>
	void DrawSprite(uint32_t playeresKey);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyManager();

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
	inline uint32_t GetKeyQuantity() {
		return uint32_t(keyes_.size());
	}


private:
	//鍵
	std::list<Key*>keyes_ = {};

	//UI
	std::unique_ptr<Sprite> keySprite_ = nullptr;
	static const uint32_t NUMBER_QUANTITY_ = 10;
	std::unique_ptr<Sprite> keyNumber[NUMBER_QUANTITY_] = { nullptr };


};

