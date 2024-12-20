#include "InstancingModel.h"

#include <numbers>
#include <cassert>

#include "Camera.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "PipelineManager.h"

#include "SrvManager.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

InstancingModel::InstancingModel() {
	//テクスチャ管理クラスの取得
	textureManager_ = TextureManager::GetInstance();

	//モデル管理クラスの取得
	modelmanager_ = ModelManager::GetInstance();

	//DirectXクラスの取得
	directXSetup_ = DirectXSetup::GetInstance();

	//パイプライン管理クラスの取得
	pipelineManager_ = PipelineManager::GetInstance();

	//SRV管理クラスも取得
	srvManager_ = SrvManager::GetInstance();

}

InstancingModel* InstancingModel::Create(const uint32_t& modelHandle) {

	//新たなModel型のインスタンスのメモリを確保
	InstancingModel* model = new InstancingModel();

	//テクスチャの読み込み
	model->textureHandle_ = model->textureManager_->LoadTexture(model->modelmanager_->GetModelData(modelHandle).textureFilePath);

	//モデルデータ
	model->modelData_ = model->modelmanager_->GetModelData(modelHandle);

	//頂点リソースを作る
	model->vertexResource_ = model->directXSetup_->CreateBufferResource(sizeof(VertexData) * model->modelmanager_->GetModelData(modelHandle).vertices.size()).Get();
	//リソースの先頭のアドレスから使う
	model->vertexBufferView_.BufferLocation = model->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	model->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * model->modelData_.vertices.size());
	//１頂点あたりのサイズ
	model->vertexBufferView_.StrideInBytes = sizeof(VertexData);



	//解析したデータを使ってResourceとBufferViewを作成する
	model->indexResource_ = model->directXSetup_->CreateBufferResource(sizeof(uint32_t) * model->modelmanager_->GetModelData(modelHandle).indices.size()).Get();
	//場所
	model->indexBufferView_.BufferLocation = model->indexResource_->GetGPUVirtualAddress();
	//サイズ
	model->indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * model->modelData_.indices.size());
	//フォーマット
	model->indexBufferView_.Format = DXGI_FORMAT_R32_UINT;



	//カメラ
	model->cameraResource_ = model->directXSetup_->CreateBufferResource(sizeof(CameraForGPU)).Get();

	return model;

}

//描画
void InstancingModel::Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material, const DirectionalLight& directionalLight) {
	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するらしいとのこと


#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

	uint32_t* mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);

#pragma region PixelShaderに送る方のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.affineMatrix_.m[3][0];
	cameraWorldPosition.y = camera.affineMatrix_.m[3][1];
	cameraWorldPosition.z = camera.affineMatrix_.m[3][2];

	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);
#pragma endregion





	//コマンドを積む

	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetModelRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetModelGraphicsPipelineState().Get());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//Material
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		textureManager_->GraphicsCommand(2, textureHandle_);
	}

	//DirectionalLight
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLight.bufferResource_->GetGPUVirtualAddress());

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	if (material.isEnviromentMap_ == true && eviromentTextureHandle_ != 0) {
		srvManager_->SetGraphicsRootDescriptorTable(8, eviromentTextureHandle_);
	}


	//DrawCall
	directXSetup_->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}

void InstancingModel::Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material, const PointLight& pointLight) {
	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するらしいとのこと


#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

	uint32_t* mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);


#pragma region PixelShaderに送る方のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.affineMatrix_.m[3][0];
	cameraWorldPosition.y = camera.affineMatrix_.m[3][1];
	cameraWorldPosition.z = camera.affineMatrix_.m[3][2];

	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);
#pragma endregion





	//コマンドを積む

	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetModelRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetModelGraphicsPipelineState().Get());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//Material
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		textureManager_->GraphicsCommand(2, textureHandle_);
	}

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());
	//PointLight
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLight.bufferResource_->GetGPUVirtualAddress());

	if (material.isEnviromentMap_ == true && eviromentTextureHandle_ != 0) {
		srvManager_->SetGraphicsRootDescriptorTable(8, eviromentTextureHandle_);
	}

	//DrawCall
	directXSetup_->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}

void InstancingModel::Draw(const WorldTransform& worldTransform, const Camera& camera, const Material& material, const SpotLight& spotLight) {
	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するらしいとのこと


#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

	uint32_t* mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);


#pragma region PixelShaderに送る方のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.affineMatrix_.m[3][0];
	cameraWorldPosition.y = camera.affineMatrix_.m[3][1];
	cameraWorldPosition.z = camera.affineMatrix_.m[3][2];

	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);
#pragma endregion





	//コマンドを積む
	//PSOの設定
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetModelRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetModelGraphicsPipelineState().Get());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//Material
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		textureManager_->GraphicsCommand(2, textureHandle_);
	}

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//SpotLight
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(7, spotLight.bufferResource_->GetGPUVirtualAddress());

	//環境マッピングの設定
	if (material.isEnviromentMap_ == true && eviromentTextureHandle_ != 0) {
		srvManager_->SetGraphicsRootDescriptorTable(8, eviromentTextureHandle_);
	}


	//DrawCall
	directXSetup_->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}
