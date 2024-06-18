#include "Dissolve.h"
#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "imgui.h"
#include "RtvManager.h"

void Dissolve::Initialize(uint32_t maskTexture){

	PipelineManager::GetInstance()->GenarateDissolvePSO();


	
	
	thresholdResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(float));
	dissolveInformation_.threshold = 0.5f;
	


	uint32_t width = (WindowsSetup::GetInstance()->GetClientWidth());
	uint32_t height = (WindowsSetup::GetInstance()->GetClientHeight());
	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 0.1f,0.1f,0.7f,1.0f };
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);
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
	
	const float RENDER_TARGET_CLEAR_VALUE[] = { 0.1f,0.1f,0.7f,1.0f };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), RENDER_TARGET_CLEAR_VALUE, 0, nullptr);


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
	ImGui::SliderFloat("threshold", &dissolveInformation_.threshold, 0.0f, 1.0f);
	ImGui::End();
#endif


	thresholdResource_->Map(0, nullptr, reinterpret_cast<void**>(&thresholdData_));
	thresholdData_->threshold = dissolveInformation_.threshold;
	thresholdResource_->Unmap(0, nullptr);
#pragma endregion


	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetDissolveRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetDissolveGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TextureManager::GetInstance()->GraphicsCommand(0, srvHandle_);
	TextureManager::GetInstance()->GraphicsCommand(1, maskTextureHandle_);

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, thresholdResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	

	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);


}
