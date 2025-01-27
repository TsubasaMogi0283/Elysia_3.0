#pragma once

#include "Vector3.h"
#include "DirectXSetup.h"


/// <summary>
/// ディゾルブ
/// </summary>
struct Dissolve{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private:
	/// <summary>
	/// データ
	/// </summary>
	struct DissolveData {
		//Edgeを使うかどうか
		bool isUseEdge;
		//Edgeの色
		Vector3 edgeColor;
		//Edgeの厚さ
		float edgeThinkness;
		//閾値
		float threshold;
	};

	//データ
	DissolveData* dissolveData = nullptr;

public:
	//リソース
	ComPtr<ID3D12Resource> resource = nullptr;

	//Edgeを使うかどうか
	bool isUseEdge;
	//Edgeの色
	Vector3 edgeColor;
	//Edgeの厚さ
	float edgeThinkness;
	//閾値
	float threshold;
	//マスクのテクスチャハンドル
	uint32_t maskTextureHandle = 0u;

	


};

