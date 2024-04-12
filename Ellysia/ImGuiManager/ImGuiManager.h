#pragma once


class ImGuiManager final{
private:
	//コンストラクタ
	ImGuiManager();

	//デストラクタ
	~ImGuiManager();

public:
	
	//コピーコンストラクタ禁止
	ImGuiManager(const ImGuiManager& imGui) = delete;

	//代入演算子を無効にする
	ImGuiManager& operator=(const ImGuiManager& imGui) = delete;

	//インスタンスを取得
	static ImGuiManager* GetInstance();

public:

	
	//初期化
	void Initialize();

	//ここからフレームが始まる
	void BeginFrame();

	//更新
	void Update();


	//描画前の処理
	void PreDraw();

	//描画
	void Draw();


	void EndFrame();


	//解放処理
	void Release();

	

private:

};