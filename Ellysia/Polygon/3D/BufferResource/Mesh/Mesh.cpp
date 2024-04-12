#include "Mesh.h"




void Mesh::Initialize(const std::vector<VertexData>& vertices){
	vertices_ = vertices;

	vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * vertices_.size());

	//読み込みのところでバッファインデックスを作った方がよさそう
	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation =vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size());
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());
	vertexResource_->Unmap(0, nullptr);
}

void Mesh::GraphicsCommand(){
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Mesh::DrawCall(int32_t instanceCount){
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), instanceCount, 0, 0);
}