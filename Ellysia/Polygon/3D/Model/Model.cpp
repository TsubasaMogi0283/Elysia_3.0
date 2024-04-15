#include "Model.h"
#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"
#include <numbers>


static uint32_t modelIndex;
std::list<ModelData> Model::modelInformationList_{};


Model::Model() {

}


Model* Model::Create(uint32_t modelHandle) {
	//新たなModel型のインスタンスのメモリを確保
	Model* model = new Model();


	PipelineManager::GetInstance()->SetModelBlendMode(1);
	PipelineManager::GetInstance()->GenerateModelPSO();



	////マテリアル用のリソースを作る。
	model->materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material)).Get();

	//テクスチャの読み込み
	model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(ModelManager::GetInstance()->GetModelData(modelHandle).material.textureFilePath);

	//頂点リソースを作る
	model->vertices_ = ModelManager::GetInstance()->GetModelData(modelHandle).vertices;


	model->vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * ModelManager::GetInstance()->GetModelData(modelHandle).vertices.size());

	//読み込みのところでバッファインデックスを作った方がよさそう
	//リソースの先頭のアドレスから使う
	model->vertexBufferView_.BufferLocation = model->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	model->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * model->vertices_.size());
	//１頂点あたりのサイズ
	model->vertexBufferView_.StrideInBytes = sizeof(VertexData);


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

	//初期は白色
	//モデル個別に色を変更できるようにこれは外に出しておく
	model->materialColor_ = { 1.0f,1.0f,1.0f,1.0f };

		

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
	std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());
	vertexResource_->Unmap(0, nullptr);

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

	//コマンドを積む

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());
	
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
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
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);
}




//デストラクタ
Model::~Model() {

}