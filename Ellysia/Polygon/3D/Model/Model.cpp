#include "Model.h"
#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"
#include <numbers>
#include <SrvManager.h>


Model::Model() {

}


Model* Model::Create(uint32_t modelHandle) {
	//新たなModel型のインスタンスのメモリを確保
	Model* model = new Model();

	//いずれSetModeBlendをなくしてGenerateModelPSOの所で指定できるようにしたい
	PipelineManager::GetInstance()->SetModelBlendMode(1);
	//Skinningするかどうか
	model->skinningResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(SkinningEnable)).Get();
	model->skinningResource_->Map(0, nullptr, reinterpret_cast<void**>(&model->skinningData_));
	model->isSkinning_.isSkinning = 1;
	model->skinningResource_->Unmap(0, nullptr);

	PipelineManager::GetInstance()->GenerateModelPSO(model->isSkinning_.isSkinning);


	//Material,DirectionalLight,PointLight,SpotLightをWorldTransformみたいにしたい
	//Setterでやるの面倒だと思った


	////マテリアル用のリソースを作る。
	model->materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material)).Get();

	//テクスチャの読み込み
	model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(ModelManager::GetInstance()->GetModelData(modelHandle).material.textureFilePath);
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


	//Lighting
	model->directionalLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLight)).Get();


	//カメラ
	model->cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();
	

	//PointLight
	model->pointLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLight)).Get();
	model->pointLightData_.color= { 1.0f,1.0f,1.0f,1.0f };
	model->pointLightData_.position = { 0.0f,0.0f,0.0f };
	model->pointLightData_.intensity = 4.0f;
	model->pointLightData_.radius = 5.0f;
	model->pointLightData_.decay = 5.0f;


	//SpotLight
	model->spotLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(SpotLight)).Get();
	model->spotLightData_.color = { 1.0f,1.0f,1.0f,1.0f };
	model->spotLightData_.position = { 2.0f,1.25f,0.0f };
	model->spotLightData_.intensity = 4.0f;
	model->spotLightData_.direction = { -1.0f,1.0f,0.0f };
	model->spotLightData_.distance = 7.0f;
	model->spotLightData_.decay = 2.0f;
	model->spotLightData_.cosFallowoffStart = 0.3f;
	model->spotLightData_.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);


	





	return model;

}



//描画
void Model::Draw(WorldTransform& worldTransform,Camera& camera) {
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

#pragma region マテリアル
	////書き込むためのアドレスを取得
	////reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = materialColor_;
	materialData_->lightingKinds = selectLighting_;
	materialData_->shininess = shininess_;
	materialData_->uvTransform = MakeIdentity4x4();

	materialResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region DirectionalLight
	

	if (selectLighting_ == Directional) {
		directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
		directionalLightData_->color = lightColor_;
		directionalLightData_->direction = lightingDirection_;
		directionalLightData_->intensity = directionalLightIntensity_;
		directionalLightResource_->Unmap(0, nullptr);
	}
	

#pragma endregion

#pragma region PixelShaderに送る方のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];

	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);
#pragma endregion

#pragma region 点光源
	//PointLight
	if (selectLighting_ == Point) {
		pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightMapData_));
		pointLightMapData_->color = pointLightData_.color;
		pointLightMapData_->position = pointLightData_.position;
		pointLightMapData_->intensity = pointLightData_.intensity;
		pointLightMapData_->radius = pointLightData_.radius;
		pointLightMapData_->decay = pointLightData_.decay;

		pointLightResource_->Unmap(0, nullptr);
	}
	

#pragma endregion

#pragma region スポットライト
	if (selectLighting_ == Spot) {
		spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightMapData_));
		spotLightMapData_->color = spotLightData_.color;
		spotLightMapData_->position = spotLightData_.position;
		spotLightMapData_->intensity = spotLightData_.intensity;
		spotLightMapData_->direction = spotLightData_.direction;
		spotLightMapData_->distance = spotLightData_.distance;
		spotLightMapData_->decay = spotLightData_.decay;
		spotLightMapData_->cosFallowoffStart = spotLightData_.cosFallowoffStart;
		spotLightMapData_->cosAngle = spotLightData_.cosAngle;
		spotLightResource_->Unmap(0, nullptr);
	}
	
	
#pragma endregion




#pragma region Skinning


#pragma endregion





	//コマンドを積む

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());
	
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	DirectXSetup::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	//Material
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource_->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_!= 0) {
		TextureManager::GraphicsCommand(textureHandle_ );
	}
	
	//DirectionalLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());
	
	//PixelShaderに送る方のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//PointLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLightResource_->GetGPUVirtualAddress());
	
	//SpotLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(7, spotLightResource_->GetGPUVirtualAddress());

	

	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}



void Model::Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster){
	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するとのこと
	skinCluster;
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


#pragma region マテリアル
	////書き込むためのアドレスを取得
	////reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = materialColor_;
	materialData_->lightingKinds = selectLighting_;
	materialData_->shininess = shininess_;
	materialData_->uvTransform = MakeIdentity4x4();

	materialResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region DirectionalLight


	if (selectLighting_ == Directional) {
		directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
		directionalLightData_->color = lightColor_;
		directionalLightData_->direction = lightingDirection_;
		directionalLightData_->intensity = directionalLightIntensity_;
		directionalLightResource_->Unmap(0, nullptr);
	}


#pragma endregion

#pragma region PixelShaderに送る方のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];

	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);
#pragma endregion

#pragma region 点光源
	//PointLight
	if (selectLighting_ == Point) {
		pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightMapData_));
		pointLightMapData_->color = pointLightData_.color;
		pointLightMapData_->position = pointLightData_.position;
		pointLightMapData_->intensity = pointLightData_.intensity;
		pointLightMapData_->radius = pointLightData_.radius;
		pointLightMapData_->decay = pointLightData_.decay;

		pointLightResource_->Unmap(0, nullptr);
	}


#pragma endregion

#pragma region スポットライト
	if (selectLighting_ == Spot) {
		spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightMapData_));
		spotLightMapData_->color = spotLightData_.color;
		spotLightMapData_->position = spotLightData_.position;
		spotLightMapData_->intensity = spotLightData_.intensity;
		spotLightMapData_->direction = spotLightData_.direction;
		spotLightMapData_->distance = spotLightData_.distance;
		spotLightMapData_->decay = spotLightData_.decay;
		spotLightMapData_->cosFallowoffStart = spotLightData_.cosFallowoffStart;
		spotLightMapData_->cosAngle = spotLightData_.cosAngle;
		spotLightResource_->Unmap(0, nullptr);
	}


#pragma endregion

#pragma region アニメーション用
	//animation;
	//Animation& animation_ = animation;
	////時刻を進める
	////計測した時間を使って可変フレーム対応した方が良い
	//animationTime_ += 1.0f / 60.0f;
	////最後までいったら最初からリピート再生
	////リピートするかしないかの設定をフラグでやった方がよさそう
	//animationTime_ = std::fmod(animationTime_, animation_.duration);
	////rootNodeのAnimationを取得
	//ModelData modelData = ModelManager::GetInstance()->GetModelData(modelHandle_);
	////NodeAnimation& rootNodeAnimation = animation.nodeAnimations["mixamorig:Head"];
	//NodeAnimation& rootNodeAnimation = animation_.nodeAnimations[modelData.rootNode.name];
	//////Armature
	////rootNodeAnimation;
	////modelData;
	//
	////指定時刻の値を取得
	//Vector3 translate = CalculationValue(rootNodeAnimation.translate.keyFrames, animationTime_);
	//Quaternion rotate = CalculationValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
	//Vector3 scale = CalculationValue(rootNodeAnimation.scale.keyFrames, animationTime_);
	//
	//Vector3 newRotate = { rotate .x,rotate.y,rotate.z };
	//animationLocalMatrix_ = MakeAffineMatrix(scale, newRotate,translate);


#pragma endregion

	//コマンドを積む

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	DirectXSetup::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Material
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource_->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(textureHandle_);
	}

	//DirectionalLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PixelShaderに送る方のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//PointLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLightResource_->GetGPUVirtualAddress());

	//SpotLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(7, spotLightResource_->GetGPUVirtualAddress());

	//Skinningするかどうか
	if (isSkinning_.isSkinning == 1) {
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(8, skinningResource_->GetGPUVirtualAddress());

	}
	

	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}




//デストラクタ
Model::~Model() {

}