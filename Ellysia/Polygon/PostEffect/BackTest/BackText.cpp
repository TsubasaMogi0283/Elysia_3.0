#include "BackText.h"
#include "imgui.h"

#include "PipelineManager.h"
#include "TextureManager.h"
#include "SrvManager.h"
#include "RtvManager.h"



void BackText::Initialize(){

	//エフェクトの種類を設定
	effectType_ = NoneEffect;

	
	
	//Effect
	effectResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(int32_t));
	
	//Vignette
	vignetteResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VignetteInformation));
	vignetteInformation_.pow = 0.8f;
	vignetteInformation_.scale = 16.0f;

	//GaussianFilter
	gaussianFilterResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(GaussianFilterInformation));
	gaussianFilterInformation_.sigma = 2.0f;


	//リソース作成
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, renderTargetClearValue_);
	
	//ハンドルの取得
	rtvHandle_= RtvManager::GetInstance()->Allocate("BackText");

	//RTV作成
	RtvManager::GetInstance()->GenarateRenderTargetView(rtvResource_, rtvHandle_);


	//SRV
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);

	
	

}

void BackText::PreDraw(){
	
	//RTの設定
	const float RENDER_TARGET_CLEAR_VALUE[] = { renderTargetClearValue_.x,renderTargetClearValue_.y,renderTargetClearValue_.z,renderTargetClearValue_.w };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	//RTVのクリア
	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(rtvHandle_), RENDER_TARGET_CLEAR_VALUE, 0, nullptr);

	//Depthのクリア
	DirectXSetup::GetInstance()->GetCommandList()->ClearDepthStencilView(
		DirectXSetup::GetInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//クライアントサイズの取得
	uint32_t width = WindowsSetup::GetInstance()->GetClientWidth();
	uint32_t height = WindowsSetup::GetInstance()->GetClientHeight();

	//ビューポート
	DirectXSetup::GetInstance()->GenarateViewport(width,height);
	
	//シザー矩形 
	DirectXSetup::GetInstance()->GenarateScissor(width, height);
	
}

void BackText::Draw(){

	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);


#ifdef _DEBUG
	ImGui::Begin("Effect");
	ImGui::SliderInt("Type",&effectType_,0,7);
	ImGui::End();
#endif

	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	effectResource_->Map(0, nullptr, reinterpret_cast<void**>(&effectTypeData_));
	//選択したEffectTypeを書き込み
	*effectTypeData_ = effectType_;

	effectResource_->Unmap(0, nullptr);


	//Vignette
	if (effectType_ == VignetteEffect) {
#ifdef _DEBUG

		ImGui::Begin("Vignette");
		ImGui::SliderFloat("Pow", &vignetteInformation_.pow, 0.1f, 10.0f);
		ImGui::SliderFloat("Scale", &vignetteInformation_.scale, 0.1f, 100.0f);

		ImGui::End();

#endif // _DEBUG

		vignetteResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteData_));
		vignetteData_->pow = vignetteInformation_.pow;
		vignetteData_->scale = vignetteInformation_.scale;

		vignetteResource_->Unmap(0, nullptr);
	}
	//GaussianFilter
	if (effectType_ == GaussianFilter3x3a ||
		effectType_ == GaussianFilter5x5a) {
#ifdef _DEBUG

		ImGui::Begin("GaussianFilter");
		ImGui::SliderFloat("Sigma", &gaussianFilterInformation_.sigma, 0.1f, 100.0f);

		ImGui::End();

#endif // _DEBUG

		gaussianFilterResource_->Map(0, nullptr, reinterpret_cast<void**>(&gaussianFilterData_));
		gaussianFilterData_->sigma=gaussianFilterInformation_.sigma;
		gaussianFilterResource_->Unmap(0, nullptr);

	}
	

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetFullScreenRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetFullScreenGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Effect
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, effectResource_->GetGPUVirtualAddress());

	//Vignette
	if (effectType_ == VignetteEffect) {
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, vignetteResource_->GetGPUVirtualAddress());
	}

	//Texture
	TextureManager::GraphicsCommand(2,srvHandle_);

	//GaussianFilter
	if (effectType_ == GaussianFilter3x3a ||
		effectType_ == GaussianFilter5x5a) {
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, gaussianFilterResource_->GetGPUVirtualAddress());
	}




	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);



	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}
