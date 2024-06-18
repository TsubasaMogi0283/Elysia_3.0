#include "LuminanceBasedOutline.h"
#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "RtvManager.h"

void LuminanceBasedOutline::Initialize(){


	//エフェクトごとにhlsl分けたい
	//いずれやる
	PipelineManager::GetInstance()->GenarateLuminanceBasedOutlinePSO();
	

	uint32_t width = (WindowsSetup::GetInstance()->GetClientWidth());
	uint32_t height = (WindowsSetup::GetInstance()->GetClientHeight());
	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 0.1f,0.1f,0.7f,1.0f };
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);
	const std::string postEffectName = "LuminanceBasedOutLine";
	rtvHandle_ = RtvManager::GetInstance()->Allocate(postEffectName);
	RtvManager::GetInstance()->GenarateRenderTargetView(rtvResource_, rtvHandle_);


	//Texture
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);
}

void LuminanceBasedOutline::PreDraw(){
	

	
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

void LuminanceBasedOutline::Draw(){
	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);



	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetLuminanceBasedOutlineRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetLuminanceBasedOutlineGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Texture
	TextureManager::GraphicsCommand(0,srvHandle_);

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	
	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}
