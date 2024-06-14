#include "ImGuiManager.h"
#include "WindowsSetup.h"
#include "DirectXSetup.h"
#include "SrvManager.h"
#include "../RtvManager/RtvManager.h"
//コンストラクタ
ImGuiManager::ImGuiManager() {
	
	
}

ImGuiManager* ImGuiManager::GetInstance() {
	//関数内static変数として宣言する
	static ImGuiManager instance;

	return &instance;
}



//メインループ前に
//初期化
void ImGuiManager::Initialize() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WindowsSetup::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		DirectXSetup::GetInstance()->GetDevice().Get(),
		DirectXSetup::GetInstance()->GetSwapChain().swapChainDesc.BufferCount,
		RtvManager::GetInstance()->GetRtvDesc().Format,
		SrvManager::GetInstance()->GetSrvDescriptorHeap().Get(),
		SrvManager::GetInstance()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		SrvManager::GetInstance()->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

}

//ここからフレームが始まる
void ImGuiManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}


//更新
//パラメータを変更したいタイミングで処理を行う
void ImGuiManager::Update() {
	//これはデモ用のUI
	//使う予定が無いのでコメントアウトします
	//ImGui::ShowDemoWindow();
}


//描画処理前
void ImGuiManager::PreDraw() {
	ImGui::Render();
}

//描画
void ImGuiManager::Draw() {
	//描画用のDescriptorの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetSrvDescriptorHeap().Get() };
	DirectXSetup::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}


//ここでフレームが終わる
void ImGuiManager::EndDraw() {
	//コマンドを積む
	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXSetup::GetInstance()->GetCommandList().Get());
}


//デストラクタ
void ImGuiManager::Release() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImGuiManager::~ImGuiManager() {

}