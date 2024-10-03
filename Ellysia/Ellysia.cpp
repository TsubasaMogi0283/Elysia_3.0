#include "Ellysia.h"

#include <PipelineManager.h>
#include "ImGuiManager.h"
#include <Input.h>
#include <TextureManager.h>
#include "SrvManager.h"
#include "RtvManager.h"
#include <Audio.h>
#include <AdjustmentItems.h>
#include <LevelDataManager.h>


Ellysia* Ellysia::GetInstance() {
	static Ellysia instance;
	return &instance;
}

void Ellysia::Initialize(){
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"静寂の霊園";
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
	HRESULT hResult = {};
	hResult=CoInitializeEx(0, COINIT_MULTITHREADED);
	//初期化に失敗したら止める
	assert(SUCCEEDED(hResult));

	//DirectX第1初期化
	DirectXSetup::GetInstance()->FirstInitialize();
	
	//SRV初期化
	SrvManager::GetInstance()->Initialize();

	//RTVの初期化
	RtvManager::GetInstance()->Initialize();

	///DirectX第2の初期化
	DirectXSetup::GetInstance()->SecondInitialize();

	
#ifdef _DEBUG
	//ImGuiManagerの初期化
	ImGuiManager::GetInstance()->Initialize();
#endif


	//パイプラインの初期化
	PipelineManager::GetInstance()->Initialize();

	//Inputの初期化
	Input::GetInstance()->Initialize();
	
	
	//Audioの初期化
	Audio::GetInstance()->Initialize();

	//JSON読み込みの初期化
	AdjustmentItems::GetInstance()->LoadFile();

	//GameManagerの初期化
	gameManager_ = new GameManager();
	gameManager_->Initialize();


}


#pragma region ゲームループ内の関数

void Ellysia::BeginFrame(){
	SrvManager::GetInstance()->PreDraw();
#ifdef _DEBUG
	ImGuiManager::GetInstance()->BeginFrame();
#endif
}

void Ellysia::Update(){
	//JSON用
	//グローバル変数の更新
	AdjustmentItems::GetInstance()->GetInstance()->Update();


	//入力の更新
	Input::GetInstance()->Update();
	
	//ゲームシーンの更新
	gameManager_->Update();
}

void Ellysia::Draw(){
	
#pragma region PostEffect
	
	
	//PostEffectの描画前処理
	gameManager_->PreDrawPostEffectFirst();
	//ポストエフェクト付きのスプライト
	gameManager_->DrawSpriteBack();

	
	

#pragma endregion

	//3Dオブジェクトの描画
	gameManager_->DrawObject3D();
	
	DirectXSetup::GetInstance()->StartDraw();

	//PostEffectの描画
	gameManager_->DrawPostEffect();


	//スプライトの描画
	gameManager_->DrawSprite();
	

	//ImGuiの描画
  #ifdef _DEBUG

	ImGuiManager::GetInstance()->Draw();
	
#endif
}


void Ellysia::EndFrame() {
#ifdef _DEBUG
	ImGuiManager::GetInstance()->EndDraw();
#endif
	//最後で切り替える
	DirectXSetup::GetInstance()->EndDraw();

	
}
#pragma endregion


void Ellysia::Release() {

	LevelDataManager::GetInstance()->Release();

	Audio::GetInstance()->Release();
#ifdef _DEBUG
	ImGuiManager::GetInstance()->Release();
#endif
	DirectXSetup::GetInstance()->Release();
	WindowsSetup::GetInstance()->Close();

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();

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

			//ESCAPE押されたら終了
			if (Input::GetInstance()->IsPushKey(DIK_ESCAPE)) {
				break;
			}


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
	delete gameManager_;
}
