#include "BackText.h"
#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "imgui.h"
#include <RtvManager.h>

void BackText::Initialize(){

	//エフェクトの種類を設定
	effectType_ = BoxFilter3x3;

	//エフェクトごとにhlsl分けたい
	//いずれやる
	PipelineManager::GetInstance()->GenarateFullScreenPSO();
	
	//Effect
	effectResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(int32_t));
	
	//Vignette
	vignetteResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VignetteInformation));
	vignetteInformation_.pow = 0.8f;
	vignetteInformation_.scale = 16.0f;

	//GaussianFilter
	gaussianFilterResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(GaussianFilterInformation));
	gaussianFilterInformation_.sigma = 2.0f;



	//Texture
	textureHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(RtvManager::GetInstance()->GetRenderTextureResource().Get(), textureHandle_);

	

}

void BackText::PreDraw(){
	
	//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	//D3D12_RESOURCE_STATE_RENDER_TARGET
	
	
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


	//ResourceとHandleはDirectX側で作った
	//いずれRTV・DSVManagerを作る

	

}

void BackText::Draw(){
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
	if (effectType_ == Vignette) {
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
	if (effectType_ == GaussianFilter3x3 ||
		effectType_ == GaussianFilter5x5) {
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
	if (effectType_ == Vignette) {
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, vignetteResource_->GetGPUVirtualAddress());
	}

	//Texture
	TextureManager::GraphicsCommand(2,textureHandle_);

	//GaussianFilter
	if (effectType_ == GaussianFilter3x3 ||
		effectType_ == GaussianFilter5x5) {
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, gaussianFilterResource_->GetGPUVirtualAddress());
	}




	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	
}

void BackText::Clear(){
	const float RENDER_TARGET_CLEAR_VALUE[] = { 1.0f,0.0f,0.0f,1.0f };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(2), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(2), RENDER_TARGET_CLEAR_VALUE, 0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->ClearDepthStencilView(
		DirectXSetup::GetInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


}

void BackText::SetResourceBarrier(){
	// Barrierを設定する
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = RtvManager::GetInstance()->GetRenderTextureResource().Get();
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// TransitionBarrierを張る
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

}

void BackText::PostDraw(){
	
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = RtvManager::GetInstance()->GetRenderTextureResource().Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);
	//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	//D3D12_RESOURCE_STATE_RENDER_TARGET


}
