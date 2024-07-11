#include "HSVFilter.h"
#include <PipelineManager.h>
#include <RtvManager.h>
#include <SrvManager.h>
#include <TextureManager.h>


void HSVFilter::Initialize(){
	//エフェクトごとにhlsl分けたい
	//いずれやる
	PipelineManager::GetInstance()->GenarateHSVFilterPSO();

	//リソース
	hsvResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(HSV));


	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 1.0f,0.0f,0.0f,1.0f };
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);

	const std::string postEffectName = "HSVFilter";
	rtvHandle_ = RtvManager::GetInstance()->Allocate(postEffectName);

	RtvManager::GetInstance()->GenarateRenderTargetView(rtvResource_, rtvHandle_);


	//Texture
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);


}

void HSVFilter::PreDraw(){
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
	DirectXSetup::GetInstance()->GenarateViewport(width, height);

	//シザー矩形 
	DirectXSetup::GetInstance()->GenarateScissor(width, height);

}

void HSVFilter::Draw(){
	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
#ifdef _DEBUG
	ImGui::Begin("BoxFilter");
	ImGui::SliderFloat("Hue", &hsv_.hue, -1.0f, 1.0f);
	ImGui::SliderFloat("Saturate", &hsv_.saturation, -1.0f, 1.0f);
	ImGui::SliderFloat("Value", &hsv_.value, -1.0f, 1.0f);
	ImGui::End();

#endif // _DEBUG



	//BoxFilterの設定
	hsvResource_->Map(0, nullptr, reinterpret_cast<void**>(&hsvData_));
	hsvData_->hue = hsv_.hue;
	hsvData_->saturation = hsv_.saturation;
	hsvData_->hue = hsv_.hue;
	hsvResource_->Unmap(0, nullptr);



	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetHSVFilterRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetHSVFilterGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Texture
	TextureManager::GraphicsCommand(0, srvHandle_);


	//HSV
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, hsvResource_->GetGPUVirtualAddress());





	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);



	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}
