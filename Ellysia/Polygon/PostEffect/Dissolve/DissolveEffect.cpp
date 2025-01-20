#include "DissolveEffect.h"
#include <imgui.h>

#include "PipelineManager.h"
#include "SrvManager.h"
#include "RtvManager.h"

Ellysia::DissolveEffect::DissolveEffect(){
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

void Ellysia::DissolveEffect::Initialize(){
	
	//PixelShaderに送る値の初期化
	dissolveResource_ = directXSetup_->CreateBufferResource(sizeof(DissolveData));
	dessolveValue_.isUseEdge = true;
	dessolveValue_.edgeThinkness = 0.04f;
	dessolveValue_.edgeColor = { 1.0f,1.0f,1.0f };
	dessolveValue_.threshold = 0.5f;


	
	renderTargetClearColor = { 0.0f,1.0f,0.0f,1.0f };
	//リソースの作成
	rtvResource_ = rtvManager_->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, renderTargetClearColor);
	//ハンドルの取得
	rtvHandle_ = rtvManager_->Allocate("Dissolve");
	//作成
	rtvManager_->GenarateRenderTargetView(rtvResource_, rtvHandle_);

	//SRV
	//ハンドルの取得
	srvHandle_ = srvManager_->Allocate();
	//作成
	srvManager_->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);

}

void Ellysia::DissolveEffect::PreDraw(){
	
	const float CLEAR_COLOR[] = { renderTargetClearColor.x,renderTargetClearColor.y,renderTargetClearColor.z,renderTargetClearColor.w };
	//RT
	directXSetup_->GetCommandList()->OMSetRenderTargets(
		1u, &rtvManager_->GetRtvHandle(rtvHandle_), false, &directXSetup_->GetDsvHandle());
	//クリア
	directXSetup_->GetCommandList()->ClearRenderTargetView(
		rtvManager_->GetRtvHandle(rtvHandle_), CLEAR_COLOR, 0u, nullptr);
	//DSV
	directXSetup_->GetCommandList()->ClearDepthStencilView(
		directXSetup_->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);

	//縦横のサイズ
	uint32_t width = windowSetup_->GetClientWidth();
	uint32_t height = windowSetup_->GetClientHeight();

	//ビューポート
	directXSetup_->GenarateViewport(width, height);
	//シザー矩形 
	directXSetup_->GenarateScissor(width, height);


}

void Ellysia::DissolveEffect::Draw(){

	//ResourceBarrierを張る
	directXSetup_->SetResourceBarrier(
		rtvResource_,
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//書き込み
	dissolveResource_->Map(0, nullptr, reinterpret_cast<void**>(&dissolveData_));
	//エッジを使うかどうか
	dissolveData_->isUseEdge = dessolveValue_.isUseEdge;
	//エッジの厚さ
	dissolveData_->edgeThinkness = dessolveValue_.edgeThinkness;
	//エッジの色
	dissolveData_->edgeColor = dessolveValue_.edgeColor;
	//閾値
	dissolveData_->threshold = dessolveValue_.threshold;
	//書き込み終了
	dissolveResource_->Unmap(0u, nullptr);

	//パイプラインの設定
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetDissolveRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetDissolveGraphicsPipelineState().Get());
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//SRV
	srvManager_->SetGraphicsRootDescriptorTable(0u, srvHandle_);
	//マスクテクスチャ
	srvManager_->SetGraphicsRootDescriptorTable(1u, maskTextureHandle_);
	//ディゾルブ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(2u, dissolveResource_->GetGPUVirtualAddress());
	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(3u, 1u, 0u, 0u);
	
	//ResourceBarrierを張る
	directXSetup_->SetResourceBarrier(
		rtvResource_,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);


}
