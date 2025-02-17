#include "Particle2D.h"

#include <cassert>
#include <numbers>
#include <algorithm>

#include "TextureManager.h"
#include "PipelineManager.h"
#include "SrvManager.h"
#include "Material.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"



Particle2D::Particle2D() {
	//テクスチャ管理クラスの取得
	textureManager_ = Ellysia::TextureManager::GetInstance();
	//DirectXクラスの取得
	directXSetup_ = Ellysia::DirectXSetup::GetInstance();
	//SRV管理クラスの取得
	srvManager_ = Ellysia::SrvManager::GetInstance();
	//パイプライン管理クラスの取得
	pipelineManager_ = Ellysia::PipelineManager::GetInstance();

}

Particle2D* Particle2D::Create(const uint32_t& moveType){
	//生成
	Particle2D* particle2D = new Particle2D();

#pragma region デフォルトの設定 
	particle2D->emitter_.count = 10;
	//0.5秒ごとに発生
	particle2D->emitter_.frequency = 0.0f;
	//発生頻度用の時刻。0.0で初期化
	particle2D->emitter_.frequencyTime = 0.0f;
	//SRT
	particle2D->emitter_.transform.scale = { .x = 20.0f,.y = 20.0f,.z = 20.0f };
	particle2D->emitter_.transform.rotate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	particle2D->emitter_.transform.translate = { .x = 0.0f,.y = 0.0f,.z = 4.0f };

	//テクスチャの読み込み
	particle2D->textureHandle_ = particle2D->textureManager_->LoadTexture("Resources/External/Texture/Circle/circle.png");

#pragma endregion


	//動きの種類
	particle2D->moveType_ = moveType;

	//頂点リソースを作る
	particle2D->vertexResource_ = particle2D->directXSetup_->CreateBufferResource(sizeof(VertexData) * 6);

	//リソースの先頭のアドレスから使う
	particle2D->vertexBufferView_.BufferLocation = particle2D->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	particle2D->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 6);
	//１頂点あたりのサイズ
	particle2D->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//インスタンシング
	particle2D->instancingResource_ = particle2D->directXSetup_->CreateBufferResource(sizeof(ParticleForGPU) * particle2D->MAX_INSTANCE_NUMBER_);

	//SRVを作る
	particle2D->instancingIndex_ = particle2D->srvManager_->Allocate();
	particle2D->srvManager_->CreateSRVForStructuredBuffer(particle2D->instancingIndex_, particle2D->instancingResource_.Get(), particle2D->MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));
	//書き込み
	particle2D->instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&particle2D->instancingData_));


	return particle2D;
}

ParticleInformation Particle2D::MakeNewParticle(std::mt19937& randomEngine){
	//ランダムの値で位置を決める
	//SRは固定
	std::uniform_real_distribution<float> distribute(-2.0f, 2.0f);
	ParticleInformation particle;
	particle.transform.scale = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
	particle.transform.rotate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	Vector3 randomTranslate = { .x = distribute(randomEngine),.y = distribute(randomEngine) + 1.0f,.z = 0.0f};
	particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, randomTranslate);
	//投げ上げは少しだけ上にずらす
	if (moveType_ == ThrowUp) {
		Vector3 offset = { .x = randomTranslate.x,.y = 0.1f,.z = 0.0f };
		particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, offset);

	}

	//速度
	std::uniform_real_distribution<float>distVelocity(-1.0f, 1.0f);
	particle.velocity = { .x = distVelocity(randomEngine),.y = distVelocity(randomEngine),.z = distVelocity(randomEngine) };

	//色
	std::uniform_real_distribution<float> distColor(1.0f, 1.0f);
	particle.color = { .x = distColor(randomEngine),.y = distColor(randomEngine),.z = distColor(randomEngine),.w = 1.0f };

	//時間
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0.0f;


	return particle;
}

std::list<ParticleInformation> Particle2D::Emission(const Emitter& emmitter, std::mt19937& randomEngine){
	emmitter;
	randomEngine;
	return std::list<ParticleInformation>();
}

void Particle2D::Update(){

}

void Particle2D::Draw(){
	//更新
	Update();



	//書き込むためのアドレスを取得
	//vertexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vertexData_));

	//float left = (0.0f - anchorPoint_.x) * size_.x;
	//float right = (1.0f - anchorPoint_.x) * size_.x;
	//float top = (0.0f - anchorPoint_.y) * size_.y;
	//float bottom = (1.0f - anchorPoint_.y) * size_.y;
	//
	//
	//
	////左下
	//vertexData_[LeftBottom].position = { left,bottom,0.0f,1.0f };
	//vertexData_[LeftBottom].texCoord = { texLeft,texBottom };
	////左上
	//vertexData_[LeftTop].position = { left,top,0.0f,1.0f };
	//vertexData_[LeftTop].texCoord = { texLeft,texTop };
	////右下
	//vertexData_[RightBottom].position = { right,bottom,0.0f,1.0f };
	//vertexData_[RightBottom].texCoord = { texRight,texBottom };
	////右上
	//vertexData_[RightTop].position = { right,top,0.0f,1.0f };
	//vertexData_[RightTop].texCoord = { texRight,texTop };
	//vertexResource_->Unmap(0u, nullptr);



	materialResource_->Map(0u, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	//ライティングしない
	materialData_->lightingKinds = NoneLighting;
	materialData_->shininess = 0.0f;

	Matrix4x4 uvTransformMatrix = Matrix4x4Calculation::MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix = Matrix4x4Calculation::Multiply(uvTransformMatrix, Matrix4x4Calculation::MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix = Matrix4x4Calculation::Multiply(uvTransformMatrix, Matrix4x4Calculation::MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix;
	materialResource_->Unmap(0u, nullptr);

	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetParticle3DRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, materialResource_->GetGPUVirtualAddress());

	//インスタンシング
	srvManager_->SetGraphicsRootDescriptorTable(1u, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0u) {
		textureManager_->GraphicsCommand(2u, textureHandle_);
	}


	//DrawCall
	directXSetup_->GetCommandList()->DrawInstanced(6u, numInstance_, 0u, 0u);


}
