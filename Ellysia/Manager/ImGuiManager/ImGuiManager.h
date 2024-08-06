#pragma once


class ImGuiManager final{
private:
	//コンストラクタ
	ImGuiManager()=default;

	//デストラクタ
	~ImGuiManager()=default;

public:
	
	//コピーコンストラクタ禁止
	ImGuiManager(const ImGuiManager& imGui) = delete;

	//代入演算子を無効にする
	ImGuiManager& operator=(const ImGuiManager& imGui) = delete;

	//インスタンスを取得
	static ImGuiManager* GetInstance();

public:

	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// フレーム開始
	/// </summary>
	void BeginFrame();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画終わり
	/// </summary>
	void EndDraw();


	/// <summary>
	/// 解放
	/// </summary>
	void Release();

	

};