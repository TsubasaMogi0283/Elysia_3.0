#include "Ellysia.h"

#include <PipelineManager.h>
#include "ImGuiManager.h"
#include <Input.h>
#include <TextureManager.h>
#include "SrvManager/SrvManager.h"
#include <Audio.h>
#include <AdjustmentItems.h>


//インスタンス
Ellysia* Ellysia::GetInstance() {
	static Ellysia instance;

	return &instance;
}

void Ellysia::Initialize(){
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"Ellysia";
	//ウィンドウのサイズを決める
	const int WINDOW_SIZE_WIDTH_ = 1280;
	const int WINDOW_SIZE_HEIGHT_ = 720;

	//初期化
	//ウィンドウ
	WindowsSetup::GetInstance()->Initialize(titleBarName,WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	
	//DirectX
	DirectXSetup::GetInstance()->FirstInitialize();
	
	//SRV初期化
	SrvManager::GetInstance()->Initialize();

	///DirectX
	DirectXSetup::GetInstance()->SecondInitialize();

	//ImGuiManager
	ImGuiManager::GetInstance()->Initialize();
	
	//Input
	Input::GetInstance()->Initialize();
	
	//TextureManager
	TextureManager::GetInstance()->Initilalize();
	
	//Audio
	Audio::GetInstance()->Initialize();

	AdjustmentItems::GetInstance()->LoadFile();

	//GameManager
	gameManager_ = new GameManager();
	gameManager_->Initialize();


	back_ = new BackText();
	back_->Initialize();
}


#pragma region ゲームループ内の関数

void Ellysia::BeginFrame(){
	SrvManager::GetInstance()->PreDraw();
	ImGuiManager::GetInstance()->BeginFrame();
}

void Ellysia::Update(){
	//JSON用
	//グローバル変数の更新
	AdjustmentItems::GetInstance()->GetInstance()->Update();

	//ImGuiの更新
	ImGuiManager::GetInstance()->Update();

	//入力の更新
	Input::GetInstance()->Update();
	
	//ゲームシーンの更新
	gameManager_->Update();
}

void Ellysia::Draw(){
	
	//OMS
	//CRTV
	//CDSV
	//RSSV
	//RSSR
	back_->PreDraw();
	
	//
	//
	////3Dオブジェクトの描画
	gameManager_->DrawObject3D();
	//
	back_->PostDraw();
	//OMSet
	//Clear
	//RSSV
	//RSSR
	DirectXSetup::GetInstance()->StartDraw();
	back_->Draw();
	//スプライトの描画
	gameManager_->DrawSprite();

	//ImGuiの描画
	ImGuiManager::GetInstance()->PreDraw();	
	ImGuiManager::GetInstance()->Draw();
	ImGuiManager::GetInstance()->EndDraw();
	
}


void Ellysia::EndFrame() {
	//最後で切り替える
	DirectXSetup::GetInstance()->EndDraw();
			
}
#pragma endregion


void Ellysia::Release() {

	Audio::GetInstance()->Release();
	TextureManager::GetInstance()->Release();
	ImGuiManager::GetInstance()->Release();
	DirectXSetup::GetInstance()->Release();
	WindowsSetup::GetInstance()->Close();
}


void Ellysia::Operate(){
	//初期化
	Initialize();
	
	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			//common_->WinMSG(msg);
			WindowsSetup::GetInstance()->WindowsMSG(msg);

		}
		else {
			//ゲームの処理
			//フレームの開始
			BeginFrame();

			//更新処理
			Update();


			//描画
			Draw();
		
			//フレームの終わり
			EndFrame();

			
		}
		
	}

	//解放
	Release();


}





Ellysia::~Ellysia(){
	delete back_;
	delete gameManager_;
}
