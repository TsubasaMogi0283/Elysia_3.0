#include "Ellysia.h"


#include "WindowsSetup.h"
#include "DirectXSetup.h"
#include "PipelineManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "SrvManager.h"
#include "RtvManager.h"
#include "Audio.h"
#include "LevelDataManager.h"
#include "GlobalVariables/GlobalVariables.h"

Ellysia::Framework::Framework(){

	//インスタンスの取得
	//ウィンドウ
	windowsSetup_ = Ellysia::WindowsSetup::GetInstance();
	//DirectX
	directXSetup_ = Ellysia::DirectXSetup::GetInstance();
	//SRV
	srvManager_ = Ellysia::SrvManager::GetInstance();
	//RTV
	rtvManager_ = Ellysia::RtvManager::GetInstance();
	//ImGui管理クラス
	imGuiManager_ = Ellysia::ImGuiManager::GetInstance();
	//パイプライン
	pipelineManager_ = Ellysia::PipelineManager::GetInstance();
	//入力
	input_ = Ellysia::Input::GetInstance();
	//Audio
	audio_ = Ellysia::Audio::GetInstance();
	//JSON読み込み
	globalVariables_ = Ellysia::GlobalVariables::GetInstance();
	//レベルデータ管理クラス
	levelDataManager_ = Ellysia::LevelDataManager::GetInstance();

}

void Ellysia::Framework::Initialize(){
	//ここでタイトルバーの名前を決めてね
	const wchar_t* TITLE_BAR_NAME = L"静寂の霊園";
	//ウィンドウのサイズを決める
	const int WINDOW_SIZE_WIDTH = 1280;
	const int WINDOW_SIZE_HEIGHT = 720;

	//初期化
	//ウィンドウ
	windowsSetup_->Initialize(TITLE_BAR_NAME,WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている
	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	HRESULT hResult=CoInitializeEx(0, COINIT_MULTITHREADED);
	//初期化に失敗したら止める
	assert(SUCCEEDED(hResult));

	//DirectX第1初期化
	directXSetup_->FirstInitialize();
	
	//SRV初期化
	srvManager_->Initialize();

	//RTVの初期化
	rtvManager_->Initialize();

	///DirectX第2の初期化
	directXSetup_->SecondInitialize();

#ifdef _DEBUG
	//ImGuiManagerの初期化
	imGuiManager_->Initialize();
#endif

	//パイプラインの初期化
	pipelineManager_->Initialize();

	//Inputの初期化
	input_->Initialize();
	
	//Audioの初期化
	audio_->Initialize();

	//JSON読み込みの初期化
	globalVariables_->LoadAllFile();

	//ゲームシーン管理クラスの生成
	gameManager_ = std::make_unique<GameManager>();
	//初期化
	gameManager_->Initialize();

}


#pragma region ゲームループ内の関数

void Ellysia::Framework::BeginFrame(){
	
	//SRVの更新
	srvManager_->PreDraw();

#ifdef _DEBUG
	//ImGuiの開始
	imGuiManager_->BeginFrame();
#endif
}

void Ellysia::Framework::Update(){

	//グローバル変数の更新
	globalVariables_->Update();

	//入力の更新
	input_->Update();
	
	//ゲームシーンの更新
	gameManager_->Update();
}

void Ellysia::Framework::Draw(){
	
	//PostEffectの描画前処理
	gameManager_->PreDrawPostEffectFirst();

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
	imGuiManager_->Draw();
	
#endif
}


void Ellysia::Framework::EndFrame() {

#ifdef _DEBUG
	////ImGuiのフレーム終わり
	imGuiManager_->EndDraw();
#endif
	//最後で切り替える
	directXSetup_->EndDraw();

}
#pragma endregion


void Ellysia::Framework::Finalize() {

	//レベルエディタの解放
	levelDataManager_->Finalize();

	//オーディオの解放
	audio_->Finalize();

#ifdef _DEBUG
	//ImGuiの解放	
	imGuiManager_->Finalize();
#endif

	//DirectXの解放
	directXSetup_->Release();
	
	//Windowsの解放
	windowsSetup_->Close();

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();

}




void Ellysia::Framework::Run(){
	//初期化
	Initialize();
	
	//メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	MSG msg = {};
	while (msg.message != WM_QUIT) {

		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			//メッセージを送る
			windowsSetup_->WindowsMSG(msg);

		}
		else {
			//ゲームの処理
			//フレームの開始
			BeginFrame();

			//更新処理
			Update();

			//ESCAPE押されたら終了
			if (input_->IsTriggerKey(DIK_ESCAPE)==true) {
				break;
			}

			//描画
			Draw();
		
			//フレームの終わり
			EndFrame();
		}
	}

	//解放
	Finalize();

}


