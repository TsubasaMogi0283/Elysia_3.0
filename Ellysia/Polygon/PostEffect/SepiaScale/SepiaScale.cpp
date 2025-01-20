#include "SepiaScale.h"
#include <imgui.h>

#include "PipelineManager.h"
#include "SrvManager.h"
#include "RtvManager.h"

Ellysia::SepiaScale::SepiaScale(){
	//ウィンドウクラスの取得
	windowSetup_ = Ellysia::WindowsSetup::GetInstance();
	//DirectXクラスの取得
	directXSetup_ = Ellysia::DirectXSetup::GetInstance();
	//パイプライン管理クラスの取得
	pipelineManager_ = Ellysia::PipelineManager::GetInstance();
	//RTV管理クラスの取得
	rtvManager_ = Ellysia::RtvManager::GetInstance();
	//SRV管理クラスの取得
	srvManager_ = Ellysia::SrvManager::GetInstance();
}

void Ellysia::SepiaScale::Initialize() {
	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 1.0f,0.0f,0.0f,1.0f };
	//リソースの生成
	rtvResource_ = rtvManager_->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);

	//ハンドルの取得
	rtvHandle_ = rtvManager_->Allocate("SepiaScale");
	//生成
	rtvManager_->GenarateRenderTargetView(rtvResource_, rtvHandle_);

	//SRV
	srvHandle_ = srvManager_->Allocate();
	srvManager_->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);

}

void Ellysia::SepiaScale::PreDraw() {

	const float RENDER_TARGET_CLEAR_VALUE[] = { 1.0f,0.0f,0.0f,1.0f };
	//RT
	directXSetup_->GetCommandList()->OMSetRenderTargets(
		1u, &rtvManager_->GetRtvHandle(rtvHandle_), false, &directXSetup_->GetDsvHandle());
	//クリア
	directXSetup_->GetCommandList()->ClearRenderTargetView(
		rtvManager_->GetRtvHandle(rtvHandle_), RENDER_TARGET_CLEAR_VALUE, 0u, nullptr);
	//DSV
	directXSetup_->GetCommandList()->ClearDepthStencilView(
		directXSetup_->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);

	//縦横
	uint32_t width = windowSetup_->GetClientWidth();
	uint32_t height = windowSetup_->GetClientHeight();

	//ビューポート
	directXSetup_->GenarateViewport(width, height);
	//シザー矩形 
	directXSetup_->GenarateScissor(width, height);

}

void Ellysia::SepiaScale::Draw() {

	//ResourceBarrierを張る
	directXSetup_->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//パイプラインの設定
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetSepiaScaleRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetSepiaScaleGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//SRV
	srvManager_->SetGraphicsRootDescriptorTable(0u, srvHandle_);
	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(3u, 1u, 0u, 0u);
	//ResourceBarrierを張る
	directXSetup_->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}
