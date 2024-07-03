#include "GaussianFilter.h"
#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "imgui.h"
#include "RtvManager.h"
#include <Input.h>



void GaussianFilter::Initialize(){


	//エフェクトごとにhlsl分けたい
	//いずれやる
	PipelineManager::GetInstance()->GenarateGaussianFilterPSO();
	
	//Effect
	gaussianFilterResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(GaussianFilterData));
	sigma_ = 2.0f;
	boxFilterType_ = GaussianFilter3x3;

	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 1.0f,0.0f,0.0f,1.0f };
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);
	
	const std::string postEffectName = "GaussianFilter";
	rtvHandle_= RtvManager::GetInstance()->Allocate(postEffectName);

	RtvManager::GetInstance()->GenarateRenderTargetView(rtvResource_, rtvHandle_);


	//Texture
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);

	
	

}

void GaussianFilter::PreDraw(){
	
	const float RENDER_TARGET_CLEAR_VALUE[] = { 1.0f,0.0f,0.0f,1.0f };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), RENDER_TARGET_CLEAR_VALUE, 0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->ClearDepthStencilView(
		DirectXSetup::GetInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	uint32_t width = WindowsSetup::GetInstance()->GetClientWidth();
	uint32_t height = WindowsSetup::GetInstance()->GetClientHeight();

	//ビューポート
	DirectXSetup::GetInstance()->GenarateViewport(width,height);
	
	//シザー矩形 
	DirectXSetup::GetInstance()->GenarateScissor(width, height);
	
}

void GaussianFilter::Draw(){

	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

#ifdef _DEBUG
	ImGui::Begin("GaussianFilter");
	ImGui::SliderInt("Type", &boxFilterType_, 0, 3);
	ImGui::SliderFloat("Sigma", &sigma_, 0.0f, 6.0f);
	ImGui::End();
#endif // _DEBUG

	if (sigma_ <= 0.0f) {
		sigma_ = 0.05f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_Q) == true) {
		sigma_ -= 0.01f;
		if (sigma_ <= 0.0f) {
			sigma_ = 0.05f;
		}
	}
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		sigma_ += 0.01f;
		if (sigma_ > 6.0f) {
			sigma_ = 6.0f;
		}
	}


	gaussianFilterResource_->Map(0, nullptr, reinterpret_cast<void**>(&gaussianFilterTypeData_));
	gaussianFilterTypeData_->sigma = sigma_;
	gaussianFilterTypeData_->type = boxFilterType_;
	gaussianFilterResource_->Unmap(0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetGaussianFilterRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetGaussianFilterGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	//Texture
	TextureManager::GraphicsCommand(0,srvHandle_);

	//Effect
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, gaussianFilterResource_->GetGPUVirtualAddress());



	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);



	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}
