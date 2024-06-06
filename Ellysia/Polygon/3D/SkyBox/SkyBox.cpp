#include "SkyBox.h"

#include "VectorCalculation.h"
#include "WorldTransform.h"
#include "Camera.h"

#include "DirectXSetup.h"
#include "PipelineManager.h"
#include <TextureManager.h>


//初期化
void SkyBox::Create(uint32_t textureHandle) {

	PipelineManager::GetInstance()->GenarateSkyBoxPSO();

	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * SURFACE_VERTEX_  * 6);
	//書き込み用のアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));


	//右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	vertexData_[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[3].position = { 1.0f,-1.0f,-1.0f,1.0f };

	//左面。描画インデックスは[4,5,6][6,5,7]で内側を向く
	vertexData_[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[7].position = { -1.0f,-1.0f,1.0f,1.0f };

	//前面。描画インデックスは[8,9,10][10,9,11]で内側を向く
	vertexData_[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData_[11].position = { 1.0f,-1.0f,1.0f,1.0f };

	//後は自力で
	//後面。
	vertexData_[12].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[13].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[14].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[15].position = { -1.0f,-1.0f,-1.0f,1.0f };

	//上面。
	vertexData_[16].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[17].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[18].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[19].position = { -1.0f,1.0f,-1.0f,1.0f };



	//下面。
	vertexData_[20].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[21].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[22].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[23].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexResource_->Unmap(0, nullptr);

	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_= DirectXSetup::GetInstance()->CreateBufferResource(sizeof(SkyBoxMaterial));
	


	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferViewSphere_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * SURFACE_VERTEX_ * 6;
	//１頂点あたりのサイズ
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);

	textureHandle_ = textureHandle;

}


//描画
//左上、右上、左下、右下
void SkyBox::Draw(WorldTransform& worldTransform, Camera& camera) {

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = {1.0f,1.0f,1.0f,1.0f};
	materialResource_->Unmap(0, nullptr);
	
	

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetSkyBoxRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetSkyBoxGraphicsPipelineState().Get());

	
	//コマンドを積む
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere_);

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldTransform.bufferResource_->GetGPUVirtualAddress());
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, camera.bufferResource_->GetGPUVirtualAddress());

	if (textureHandle_ != 0) {
		TextureManager::GetInstance()->GraphicsCommand(textureHandle_);
	}

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, materialResource_->GetGPUVirtualAddress());

	

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(SURFACE_VERTEX_ *6, 1, 0, 0);

	


}

