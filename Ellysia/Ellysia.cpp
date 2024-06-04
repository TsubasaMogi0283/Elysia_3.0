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
	
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている

	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	//Textureを読むにあたって、COMコンポーネントの１つを利用する
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//DirectX
	DirectXSetup::GetInstance()->Initialize();
	
	SrvManager::GetInstance()->Initialize();

	//ImGuiManager
	ImGuiManager::GetInstance()->Initialize();
	
	//Input
	Input::GetInstance()->Initialize();
	
	
	//Audio
	Audio::GetInstance()->Initialize();

	AdjustmentItems::GetInstance()->LoadFile();

	//GameManager
	gameManager_ = new GameManager();
	gameManager_->Initialize();

}


#pragma region ゲームループ内の関数

void Ellysia::BeginFrame(){
	DirectXSetup::GetInstance()->BeginFrame();
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
	//ImGuiの描画
	ImGuiManager::GetInstance()->PreDraw();	
	ImGuiManager::GetInstance()->Draw();
	
	//ゲームシーンの描画
	gameManager_->Draw();
}


void Ellysia::EndFrame() {

	ImGuiManager::GetInstance()->EndFrame();
	DirectXSetup::GetInstance()->EndFrame();
			
}
#pragma endregion


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

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();

}



void Ellysia::Release() {

	Audio::GetInstance()->Release();
	ImGuiManager::GetInstance()->Release();
	DirectXSetup::GetInstance()->Release();
	WindowsSetup::GetInstance()->Close();
}

Ellysia::~Ellysia(){
	delete gameManager_;
}
