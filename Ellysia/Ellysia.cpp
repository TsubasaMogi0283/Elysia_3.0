#include "Ellysia.h"

#include <PipelineManager.h>
#include "ImGuiManager.h"
#include <Input.h>
#include <TextureManager.h>
#include "SrvManager.h"
#include "RtvManager.h"
#include <Audio.h>
#include <LevelDataManager.h>
#include "GlobalVariables/GlobalVariables.h"

Ellysia* Ellysia::GetInstance() {
	static Ellysia instance;
	return &instance;
}

void Ellysia::Initialize(){
	//ここでタイトルバーの名前を決めてね
	const wchar_t* TITLE_BAR_NAME = L"静寂の霊園";
	//ウィンドウのサイズを決める
	const int WINDOW_SIZE_WIDTH = 1280;
	const int WINDOW_SIZE_HEIGHT = 720;



	//初期化
	//ウィンドウ
	WindowsSetup::GetInstance()->Initialize(TITLE_BAR_NAME,WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている
	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	HRESULT hResult=CoInitializeEx(0, COINIT_MULTITHREADED);
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
	GlobalVariables::GetInstance()->LoadFile();

	//GameManagerの初期化
	gameManager_ = std::make_unique<GameManager>();
	gameManager_->Initialize();


}


#pragma region ゲームループ内の関数

void Ellysia::BeginFrame(){
	
	//SRVの更新
	SrvManager::GetInstance()->PreDraw();

#ifdef _DEBUG
	//ImGuiの開始
	ImGuiManager::GetInstance()->BeginFrame();
#endif
}

void Ellysia::Update(){
	//JSON用
	//グローバル変数の更新
	GlobalVariables::GetInstance()->GetInstance()->Update();


	//入力の更新
	Input::GetInstance()->Update();
	
	//ゲームシーンの更新
	gameManager_->Update();
}

void Ellysia::Draw(){
	
#pragma region PostEffect
	
	
	//PostEffectの描画前処理
	gameManager_->PreDrawPostEffectFirst();

	

#pragma endregion

	//3Dオブジェクトの描画
	gameManager_->DrawObject3D();
	
	//描画始め(スワップチェイン)
	DirectXSetup::GetInstance()->StartDraw();

	//PostEffectの描画
	gameManager_->DrawPostEffect();


	//スプライトの描画
	gameManager_->DrawSprite();
	

	
#ifdef _DEBUG
	//ImGuiの描画
	ImGuiManager::GetInstance()->Draw();
	
#endif
}


void Ellysia::EndFrame() {
#ifdef _DEBUG
	////ImGuiのフレーム終わり
	ImGuiManager::GetInstance()->EndDraw();
#endif
	//最後で切り替える
	DirectXSetup::GetInstance()->EndDraw();

	
}
#pragma endregion


void Ellysia::Release() {

	//レベルエディタの解放
	LevelDataManager::GetInstance()->Release();

	//オーディオの解放
	Audio::GetInstance()->Release();

#ifdef _DEBUG
	//ImGuiの解放	
	ImGuiManager::GetInstance()->Release();
#endif

	//DirectXの解放
	DirectXSetup::GetInstance()->Release();
	
	//Windowsの解放
	WindowsSetup::GetInstance()->Close();

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();

}


void Ellysia::Run(){
	//初期化
	Initialize();
	
	

	//メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	MSG msg = {};
	while (msg.message != WM_QUIT) {

		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			//メッセージを送る
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


