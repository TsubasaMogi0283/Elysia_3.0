#include "AnimationModel.h"
#include <PipelineManager.h>
#include <TextureManager.h>
#include "SkinCluster.h"
#include <ModelManager.h>
#include <numbers>
#include <Matrix4x4Calculation.h>


#include <SrvManager.h>
#include "WorldTransform.h"
#include "Material.h"
#include <Camera.h>
#include <SpotLight.h>
#include <PointLight.h>
#include <DirectionalLight.h>


AnimationModel* AnimationModel::Create(uint32_t modelHandle){
	//新たなModel型のインスタンスのメモリを確保
	AnimationModel* model = new AnimationModel();

	//テクスチャの読み込み
	model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(ModelManager::GetInstance()->GetModelData(modelHandle).textureFilePath);
	//Drawでも使いたいので取り入れる
	model->modelHandle_ = modelHandle;

	//モデルデータ
	model->modelData_ = ModelManager::GetInstance()->GetModelData(modelHandle);

	//頂点リソースを作る
	model->vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * ModelManager::GetInstance()->GetModelData(modelHandle).vertices.size()).Get();

	//読み込みのところでバッファインデックスを作った方がよさそう
	//リソースの先頭のアドレスから使う
	model->vertexBufferView_.BufferLocation = model->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	model->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * model->modelData_.vertices.size());
	//１頂点あたりのサイズ
	model->vertexBufferView_.StrideInBytes = sizeof(VertexData);



	//解析したデータを使ってResourceとBufferViewを作成する
	model->indexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(uint32_t) * ModelManager::GetInstance()->GetModelData(modelHandle).indices.size()).Get();
	model->indexBufferView_.BufferLocation = model->indexResource_->GetGPUVirtualAddress();
	size_t indicesSize = model->modelData_.indices.size();
	model->indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indicesSize);
	model->indexBufferView_.Format = DXGI_FORMAT_R32_UINT;



	//カメラ
	model->cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();

	return model;

	
}

void AnimationModel::Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, Material& material, DirectionalLight& directionalLight){
	//Materialのライティングの設定が平行光源ではない場合止める
	assert(material.lightingKinds_ == Directional);

	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するとのこと
#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region Index描画

	uint32_t* mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);

#pragma endregion



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

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetAnimationModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetAnimationModelGraphicsPipelineState().Get());


	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		//VertexDataのVBV
		vertexBufferView_,

		//InfluenceのVBV
		skinCluster.influenceBufferView_
	};

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
	//IBVを設定
	DirectXSetup::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Material
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2,textureHandle_);
	}

	//DirectionalLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLight.bufferResource_->GetGPUVirtualAddress());

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//paletteSrvHandle
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(8, skinCluster.paletteSrvHandle_.second);


	if (material.isEnviromentMap_==true && eviromentTextureHandle_ != 0) {
		SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(9, eviromentTextureHandle_);
	}


	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}

void AnimationModel::Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, Material& material, PointLight& pointLight){
	
	//Materialのライティングの設定が点光源ではない場合止める
	assert(material.lightingKinds_ == Point);

	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するとのこと
#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region Index描画

	uint32_t* mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);

#pragma endregion

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
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetAnimationModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetAnimationModelGraphicsPipelineState().Get());


	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		//VertexDataのVBV
		vertexBufferView_,

		//InfluenceのVBV
		skinCluster.influenceBufferView_
	};

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
	//IBVを設定
	DirectXSetup::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Material
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2, textureHandle_);
	}


	

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//PointLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLight.bufferResource_->GetGPUVirtualAddress());


	//paletteSrvHandle
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(8, skinCluster.paletteSrvHandle_.second);


	if (material.isEnviromentMap_ == true && eviromentTextureHandle_ != 0) {


		SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(9, eviromentTextureHandle_);
	}




	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

void AnimationModel::Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster, Material& material, SpotLight& spotLight){
	
	//Materialのライティングの設定がスポットライトではない場合止める
	assert(material.lightingKinds_ == Spot);
	
	// 資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するとのこと
#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData * vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region Index描画

	uint32_t* mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);

#pragma endregion


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
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetAnimationModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetAnimationModelGraphicsPipelineState().Get());


	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		//VertexDataのVBV
		vertexBufferView_,

		//InfluenceのVBV
		skinCluster.influenceBufferView_
	};

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
	//IBVを設定
	DirectXSetup::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Material
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2, textureHandle_);
	}




	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//SpotLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(7, spotLight.bufferResource_->GetGPUVirtualAddress());
	

	//paletteSrvHandle
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(8, skinCluster.paletteSrvHandle_.second);



	if (material.isEnviromentMap_ == true && eviromentTextureHandle_ != 0) {
		SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(9, eviromentTextureHandle_);
	}



	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}
