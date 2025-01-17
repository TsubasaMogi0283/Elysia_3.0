#include "Vignette.h"

#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "imgui.h"
#include "RtvManager.h"



Vignette::Vignette(){
	//ウィンドウクラスの取得
	windowsSetup_ = Ellysia::WindowsSetup::GetInstance();
	//DirectXクラスの取得
	directXSetup_ = Ellysia::DirectXSetup::GetInstance();
	//パイプライン管理クラスの取得
	pipelinemanager_ = Ellysia::PipelineManager::GetInstance();
	//RTV管理クラスの取得
	rtvManager_ = Ellysia::RtvManager::GetInstance();
	//SRV管理クラスの取得
	srvManager_ = Ellysia::SrvManager::GetInstance();;
}

void Vignette::Initialize() {

	
	//RTV用のリソースを生成
	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 0.0f,0.0f,0.0f,1.0f };
	rtvResource_ = rtvManager_->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);
	//ハンドルの取得
	rtvHandle_ = rtvManager_->Allocate("Vignette");
	//RTVの生成
	rtvManager_->GenarateRenderTargetView(rtvResource_, rtvHandle_);

	//ハンドルの取得
	srvHandle_ = srvManager_->Allocate();
	//SRVの生成
	srvManager_->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);

	
	//リソースの生成
	valueResource_ = directXSetup_->CreateBufferResource(sizeof(VignetteData));
	//初期化
	scale_ = 16.0f;
	pow_ = 0.8f;
	color_ = { .x = 0.0f,.y = 0.0f,.z = 1.0f };


}

void Vignette::PreDraw() {

	const float RENDER_TARGET_CLEAR_VALUE[] = { 0.0f,0.0f,0.0f,1.0f };
	directXSetup_->GetCommandList()->OMSetRenderTargets(
		1u, &rtvManager_->GetRtvHandle(rtvHandle_), false, &directXSetup_->GetDsvHandle());

	directXSetup_->GetCommandList()->ClearRenderTargetView(
		rtvManager_->GetRtvHandle(rtvHandle_), RENDER_TARGET_CLEAR_VALUE, 0u, nullptr);


	directXSetup_->GetCommandList()->ClearDepthStencilView(
		directXSetup_->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);

	uint32_t width = windowsSetup_->GetClientWidth();
	uint32_t height = windowsSetup_->GetClientHeight();

	//ビューポート
	directXSetup_->GenarateViewport(width, height);

	//シザー矩形 
	directXSetup_->GenarateScissor(width, height);

}

void Vignette::Draw() {

	//ResourceBarrierを張る
	directXSetup_->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//書き込みと転送
	Transfer();

	//PSOの設定
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelinemanager_->GetVignetteRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelinemanager_->GetVignetteGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SRV
	srvManager_->SetGraphicsRootDescriptorTable(0u, srvHandle_);

	//数値をGPUへ送る
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, valueResource_->GetGPUVirtualAddress());

	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(3u, 1u, 0u, 0u);

	//ResourceBarrierを張る
	directXSetup_->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}

void Vignette::Transfer(){
	//数値の書き込み
	valueResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vignetteData_));
	vignetteData_->scale = scale_;
	vignetteData_->pow = pow_;
	vignetteData_->color = color_;
	valueResource_->Unmap(0u, nullptr);
}
