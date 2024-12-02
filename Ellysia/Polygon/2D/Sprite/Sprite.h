#pragma once
/**
 * @file GameManager.h
 * @brief スプライトのクラス
 * @author 茂木翼
 */

#include <string>
#include <format>


#include "DirectXSetup.h"
#include "Vector4.h"
#include "Vector2.h"
#include "Transform.h"
#include "VertexData.h"
#include "Material.h"
#include "TransformationMatrix.h"


/// <summary>
/// スプライト
/// </summary>
class Sprite {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite()=default;

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	static Sprite* Create(const uint32_t& textureHandle,const Vector2& position);


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite()=default;


	
public:
	/// <summary>
	/// 透明度の設定
	/// </summary>
	/// <param name="transparency"></param>
	inline void SetTransparency(const float& transparency) {
		this->color_.w = transparency;
	}

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	inline void SetColor(const Vector4& color) {
		this->color_ = color;
	}



	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector2& scale) {
		this->scale_ = scale;
	}




	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(const float& rotate) {
		this->rotate_ = rotate;
	}



	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) {
		this->position_ = position;
	}



	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="point"></param>
	void SetAnchorPoint(Vector2 point) {
		this->anchorPoint_ = point;
	}
	


	/// <summary>
	/// X軸フリップ
	/// </summary>
	/// <param name="isFlipX"></param>
	void SetFlipX(const bool& isFlipX){
		this->isFlipX_=isFlipX;
	}

	/// <summary>
	/// Y軸フリップ
	/// </summary>
	/// <param name="isFlipY"></param>
	void SetFlipY(const bool& isFlipY){
		this->isFlipY_=isFlipY;
	}


	/// <summary>
	/// 見えなくするかどうか
	/// </summary>
	/// <param name="isInvisible"></param>
	void SetInvisible(const bool& isInvisible) {
		this->isInvisible_ = isInvisible;
	}


	/// <summary>
	/// 後ろにするかどうか
	/// </summary>
	/// <param name="isBack"></param>
	void SetIsBack(const bool& isBack) {
		this->isBack_ = isBack;
	}


	/// <summary>
	/// UV座標系左上の設定
	/// </summary>
	/// <param name="textureLeftTop"></param>
	void SetTextureLeftTop(const Vector2& textureLeftTop) {
		this->textureLeftTop_ = textureLeftTop;
	}

	/// <summary>
	/// テクスチャのサイズの設定
	/// </summary>
	/// <param name="textureSize"></param>
	void SetTextureSize(const Vector2& textureSize) {
		this->textureSize_ = textureSize;
	}

	/// <summary>
	/// UV編集モードにするかどうか
	/// </summary>
	/// <param name="isUVMode"></param>
	void SetUVMode(const bool& isUVMode) {
		this->isUVSetting_ = isUVMode;
	}





private:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <param name="position"></param>
	void Initialize(const uint32_t& textureHandle,const Vector2& position);
	

private:


	//頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点データ
	VertexData* vertexData_ = nullptr;
	//バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};


	//マテリアル
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	MaterialData* materialData_ = nullptr;
	


	//TransformationMatrix用リソース
	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	
	
	//Index用
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	//バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	//インデックスデータ
	uint32_t* indexData_ = nullptr;


	//テクスチャの情報
	D3D12_RESOURCE_DESC resourceDesc_{};


private:

	/// <summary>
	/// 頂点の位置
	/// </summary>
	enum VERTEX_POSITION {
		//左下
		LeftBottom,
	
		//左上
		LeftTop,
	
		//右下
		RightBottom,
	
		//右上
		RightTop,

	};

	//UVトランスフォーム
	Transform uvTransform_ = {};


	
	
	//サイズ
	Vector2 size_ = {};

	//S
	Vector2 scale_ = { 1.0f,1.0f };
	//R
	float rotate_ = 0.0f;
	//T
	Vector2 position_ = {};

	//アンカーポイント
	Vector2 anchorPoint_ = {};
	//色
	Vector4 color_ = {};

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;


	//非表示
	bool isInvisible_ = false;


	//手前にするか後ろにするか
	//基本手前にする
	bool isBack_ = false;

	//テクスチャ範囲設定
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 100.0f,100.0f };

	//UVの設定をするかどうか
	bool isUVSetting_ = false;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};