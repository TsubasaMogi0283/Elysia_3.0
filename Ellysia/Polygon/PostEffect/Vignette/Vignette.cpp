#include "Vignette.h"

#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "imgui.h"
#include "RtvManager.h"



void Vignette::Initialize() {

	

	const Vector4 RENDER_TARGET_CLEAR_VALUE = { 0.0f,0.0f,0.0f,1.0f };
	rtvResource_ = RtvManager::GetInstance()->CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, RENDER_TARGET_CLEAR_VALUE);

	const std::string postEffectName = "Vignette";
	rtvHandle_ = RtvManager::GetInstance()->Allocate(postEffectName);

	RtvManager::GetInstance()->GenarateRenderTargetView(rtvResource_, rtvHandle_);


	//Texture
	srvHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(rtvResource_.Get(), srvHandle_);

	
	//リソースの生成
	valueResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VignetteData));
	//初期化
	scale_ = 16.0f;
	pow_ = 0.8f;
	color_ = { .x = 0.0f,.y = 0.0f,.z = 1.0f };


}

void Vignette::PreDraw() {

	const float RENDER_TARGET_CLEAR_VALUE[] = { 0.0f,0.0f,0.0f,1.0f };
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

void Vignette::Draw() {

	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

#ifdef _DEBUG
	ImGui::Begin("Vignette");
	ImGui::SliderFloat("Scale", &scale_, 0.0f, 100.0f);
	ImGui::SliderFloat("Pow", &pow_, 0.01f, 10.0f);
	ImGui::SliderFloat3("color", &color_.x, 0.0f, 1.0f);

	ImGui::End();

#endif



#pragma region 数値の書き込み

	valueResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteData_));
	vignetteData_->scale = scale_;
	vignetteData_->pow = pow_;
	vignetteData_->color = color_;
	valueResource_->Unmap(0, nullptr);


#pragma endregion



	//PSOの設定
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetVignetteRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetVignetteGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Texture
	TextureManager::GraphicsCommand(0, srvHandle_);

	//数値をGPUへ送る
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, valueResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);



	//ResourceBarrierを張る
	DirectXSetup::GetInstance()->SetResourceBarrier(
		rtvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}
