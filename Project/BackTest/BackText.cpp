#include "BackText.h"
#include <PipelineManager.h>
#include "TextureManager.h"

void BackText::Initialize(){
	PipelineManager::GetInstance()->GenarateCopyImagePSO();
	
	//ここでBufferResourceを作る
	vertexResouce_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * 3);
	

	centerResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Vector3)*3);

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Vector4) * 3);


	textureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/White.png");
	
}

void BackText::Update(){

}

void BackText::Draw(){
	//左上
	vertexData_[0].position = {0.0f,0.0f,0.0f,1.0f};
	vertexData_[0].texCoord = { 0.0f,1.0f};
	//右上
	vertexData_[1].position = {0.2f,0.0f,0.0f,1.0f};
	vertexData_[1].texCoord = { 0.5f,0.0f };
	//左下
	vertexData_[2].position = {0.0f,-0.5f,0.0f,1.0f};
	vertexData_[2].texCoord = { 1.0f,1.0f };
	//範囲外は危険だよ！！

	centerResource_->Map(0, nullptr, reinterpret_cast<void**>(&positiondata_));
	*positiondata_ = centerPosition_;

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//今回は赤を書き込んでみる
	*materialData_ = color_;

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetCopyImageRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetCopyImageGraphicsPipelineState().Get());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	//ここでの[0]はregisterの0ではないよ。rootParameter配列の0番目
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//Position
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, centerResource_->GetGPUVirtualAddress());

	//Texture
	TextureManager::GraphicsCommand(textureHandle_);

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);



}
