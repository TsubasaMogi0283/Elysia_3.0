#include "RandomEffect.h"
#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "imgui.h"
#include "../RtvManager/RtvManager.h"


void RandomEffect::Initialize() {

	PipelineManager::GetInstance()->GenarateRandomEffectPSO();


	//Texture
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(RtvManager::GetInstance()->GetRandomEffectTextureResource().Get(), srvHandle_);
	//乱数生成の初期化
	std::random_device rand;
	std::mt19937 randomEngine(rand());
	randomEngine_ = randomEngine;


	randomValueResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(RandomValue));
	std::uniform_real_distribution<float> distribute(0.0f, 1.0f);
	randomValue_.value = distribute(randomEngine_);

	

}

void RandomEffect::PreDraw() {
	

	const float RENDER_TARGET_CLEAR_VALUE[] = { 0.1f,0.1f,0.7f,1.0f };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(6), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(6), RENDER_TARGET_CLEAR_VALUE, 0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->ClearDepthStencilView(
		DirectXSetup::GetInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = float(WindowsSetup::GetInstance()->GetClientWidth());
	viewport.Height = float(WindowsSetup::GetInstance()->GetClientHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	DirectXSetup::GetInstance()->GetCommandList()->RSSetViewports(1, &viewport);


	//シザー矩形 
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WindowsSetup::GetInstance()->GetClientWidth();
	scissorRect.top = 0;
	scissorRect.bottom = WindowsSetup::GetInstance()->GetClientHeight();

	DirectXSetup::GetInstance()->GetCommandList()->RSSetScissorRects(1, &scissorRect);




}

void RandomEffect::Draw() {
	// Barrierを設定する
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = RtvManager::GetInstance()->GetRandomEffectTextureResource().Get();
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// TransitionBarrierを張る
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);


#pragma region 閾値
#ifdef _DEBUG
	ImGui::Begin("RandomEffect");
	ImGui::InputFloat("threshold", &randomValue_.value);
	ImGui::End();
#endif
	randomValueResource_->Map(0,nullptr,reinterpret_cast<void**>(&randomValueData_));
	std::uniform_real_distribution<float> distribute(0.0f, 1.0f);
	randomValue_.value = distribute(randomEngine_);
	randomValueData_->value = randomValue_.value;
	randomValueResource_->Unmap(0, nullptr);


#pragma endregion


	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetRandomEffectRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetRandomEffectGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TextureManager::GetInstance()->GraphicsCommand(0, srvHandle_);

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, randomValueResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = RtvManager::GetInstance()->GetRandomEffectTextureResource().Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

}

void RandomEffect::PreDrawSecond() {

	
}
