#include "Dissolve.h"
#include "imgui.h"

#include <PipelineManager.h>
#include "TextureManager.h"
#include "SrvManager.h"
#include "RtvManager.h"

void Dissolve::Initialize(const uint32_t& maskTexture){
	


	
	//PixelShaderに送る値の初期化
	dissolveResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DissolveData));
	dessolveValue_.isUseEdge = true;
	dessolveValue_.edgeThinkness = 0.04f;
	dessolveValue_.edgeColor = { 1.0f,1.0f,1.0f };
	dessolveValue_.threshold = 0.5f;



	//Resourceを作る
	renderTargetClearColor = { 0.0f,1.0f,0.0f,1.0f };
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, renderTargetClearColor);
	const std::string postEffectName = "Dissolve";
	rtvHandle_ = RtvManager::GetInstance()->Allocate(postEffectName);
	RtvManager::GetInstance()->GenarateRenderTargetView(rtvResource_, rtvHandle_);

	//Texture
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);



	//マスクテクスチャ
	maskTextureHandle_ = maskTexture;

}

void Dissolve::PreDraw(){
	
	const float CLEAR_COLOR[] = { renderTargetClearColor.x,renderTargetClearColor.y,renderTargetClearColor.z,renderTargetClearColor.w };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), CLEAR_COLOR, 0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->ClearDepthStencilView(
		DirectXSetup::GetInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	uint32_t width = WindowsSetup::GetInstance()->GetClientWidth();
	uint32_t height = WindowsSetup::GetInstance()->GetClientHeight();

	//ビューポート
	DirectXSetup::GetInstance()->GenarateViewport(width, height);

	//シザー矩形 
	DirectXSetup::GetInstance()->GenarateScissor(width, height);


}

void Dissolve::Draw(){

	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_,
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);



#pragma region 閾値
#ifdef _DEBUG
	ImGui::Begin("Dissolve");
	ImGui::Checkbox("IsUseEdge", &dessolveValue_.isUseEdge);
	ImGui::SliderFloat("Thinkness", &dessolveValue_.edgeThinkness, 0.0f, 1.0f);
	ImGui::SliderFloat3("EdgeColor", &dessolveValue_.edgeColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Threshold", &dessolveValue_.threshold, 0.0f, 1.0f);

	ImGui::End();
#endif


	dissolveResource_->Map(0, nullptr, reinterpret_cast<void**>(&dissolveData_));
	dissolveData_->isUseEdge = dessolveValue_.isUseEdge;
	dissolveData_->edgeThinkness = dessolveValue_.edgeThinkness;
	dissolveData_->edgeColor = dessolveValue_.edgeColor;
	dissolveData_->threshold = dessolveValue_.threshold;

	dissolveResource_->Unmap(0, nullptr);
#pragma endregion


	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetDissolveRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetDissolveGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TextureManager::GetInstance()->GraphicsCommand(0, srvHandle_);
	TextureManager::GetInstance()->GraphicsCommand(1, maskTextureHandle_);

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, dissolveResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	

	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);


}
