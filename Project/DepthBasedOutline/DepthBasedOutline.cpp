#include "DepthBasedOutline.h"
#include <PipelineManager.h>
#include "TextureManager.h"
#include <SrvManager.h>
#include "RtvManager.h"
#include "Camera.h"
#include <Matrix4x4Calculation.h>

void DepthBasedOutline::Initialize() {


	//エフェクトごとにhlsl分けたい
	//いずれやる
	PipelineManager::GetInstance()->GenarateDepthBasedOutlinePSO();

	projectionInverseResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraMatrix)).Get();
	

	//Texture
	textureHandle_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForRenderTexture(RtvManager::GetInstance()->GetDepthBasedOutlineResource().Get(), textureHandle_);
	srvforDepthHandle_= SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForDepthTexture(srvforDepthHandle_);

}

void DepthBasedOutline::PreDraw() {
	//ResourceとHandleはDirectX側で作った
	//いずれRTV・DSVManagerを作る

	// Barrierを設定する
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = RtvManager::GetInstance()->GetDepthBasedOutlineResource().Get();
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//D3D12_RESOURCE_STATE_DEPTH_WRITE
	//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE

	// TransitionBarrierを張る
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	const float RENDER_TARGET_CLEAR_VALUE[] = { 0.1f,0.1f,0.7f,1.0f };
	DirectXSetup::GetInstance()->GetCommandList()->OMSetRenderTargets(
		1, &RtvManager::GetInstance()->GetRtvHandle(4), false, &DirectXSetup::GetInstance()->GetDsvHandle());

	DirectXSetup::GetInstance()->GetCommandList()->ClearRenderTargetView(
		RtvManager::GetInstance()->GetRtvHandle(4), RENDER_TARGET_CLEAR_VALUE, 0, nullptr);


	DirectXSetup::GetInstance()->GetCommandList()->ClearDepthStencilView(
		DirectXSetup::GetInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	uint32_t width = WindowsSetup::GetInstance()->GetClientWidth();
	uint32_t height = WindowsSetup::GetInstance()->GetClientHeight();

	//ビューポート
	DirectXSetup::GetInstance()->GenarateViewport(width, height);

	//シザー矩形 
	DirectXSetup::GetInstance()->GenarateScissor(width, height);

}

void DepthBasedOutline::Draw(Camera& camera) {

	projectionInverseResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraMatrix_));
	Matrix4x4 projectionInverseMatrix4x4 = Inverse(camera.projectionMatrix_);
	cameraMatrix_->projectionInverse = projectionInverseMatrix4x4;
	projectionInverseResource_->Unmap(0, nullptr);

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetDepthBasedOutlineRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetDepthBasedOutlineGraphicsPipelineState().Get());

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Texture
	TextureManager::GraphicsCommand(0, textureHandle_);

	//Camera
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, projectionInverseResource_->GetGPUVirtualAddress());

	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, srvforDepthHandle_);

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);


}

void DepthBasedOutline::PreDrawSecond() {

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = RtvManager::GetInstance()->GetDepthBasedOutlineResource().Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	DirectXSetup::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	//D3D12_RESOURCE_STATE_DEPTH_WRITE
	//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE


}
