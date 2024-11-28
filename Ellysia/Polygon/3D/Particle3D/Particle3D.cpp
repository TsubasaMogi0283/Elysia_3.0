#include "Particle3D.h" 

#include <cassert>
#include <numbers>

#include "Camera.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "DirectXSetup.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "VectorCalculation.h"
#include "SrvManager.h"


Particle3D::Particle3D() {
	modelManager_ = ModelManager::GetInstance();
}

Particle3D* Particle3D::Create(const uint32_t& moveType){
	Particle3D* particle3D = new Particle3D();

#pragma region デフォルトの設定 
	particle3D->emitter_.count = 100;
	//0.5秒ごとに発生
	particle3D->emitter_.frequency = 0.0f;
	//発生頻度用の時刻。0.0で初期化
	particle3D->emitter_.frequencyTime = 0.0f;
	//SRT
	particle3D->emitter_.transform.scale = { 10.0f,10.0f,10.0f };
	particle3D->emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	particle3D->emitter_.transform.translate = { 0.0f,0.0f,4.0f };

#pragma endregion

	//モデルの読み込み
	uint32_t modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/External/Model/Plane", "plane.obj");
	ModelData modelData = ModelManager::GetInstance()->GetModelData(modelHandle);

	//テクスチャの読み込み
	particle3D->textureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/External/Texture/Circle/circle.png");

	//動きの種類
	particle3D->moveType_ = moveType;

	//頂点リソースを作る
	particle3D->vertices_ = ModelManager::GetInstance()->GetModelData(modelHandle).vertices;
	particle3D->vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * particle3D->vertices_.size());



	//リソースの先頭のアドレスから使う
	particle3D->vertexBufferView_.BufferLocation = particle3D->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	particle3D->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * particle3D->vertices_.size());
	//１頂点あたりのサイズ
	particle3D->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	particle3D->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//
	std::memcpy(vertexData, particle3D->vertices_.data(), sizeof(VertexData) * particle3D->vertices_.size());
	particle3D->vertexResource_->Unmap(0, nullptr);



	//インスタンシング
	particle3D->instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);

	//SRVを作る
	particle3D->instancingIndex_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForStructuredBuffer(particle3D->instancingIndex_, particle3D->instancingResource_.Get(), MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));
	//書き込み
	particle3D->instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&particle3D->instancingData_));


	//カメラ
	particle3D->cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Vector3));


	return particle3D;

}

Particle3D* Particle3D::Create(const uint32_t& modelHandle,const uint32_t& moveType) {
	Particle3D* particle3D = new Particle3D();

#pragma region デフォルトの設定 
	particle3D->emitter_.count = 100;
	//0.5秒ごとに発生
	particle3D->emitter_.frequency = 0.0f;
	//発生頻度用の時刻。0.0で初期化
	particle3D->emitter_.frequencyTime = 0.0f;
	//SRT
	particle3D->emitter_.transform.scale = { 10.0f,10.0f,10.0f };
	particle3D->emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	particle3D->emitter_.transform.translate = { 0.0f,0.0f,4.0f };

#pragma endregion

	//テクスチャの読み込み
	ModelData modelData = ModelManager::GetInstance()->GetModelData(modelHandle);


	particle3D->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.textureFilePath);

	//動きの種類
	particle3D->moveType_ = moveType;

	//頂点リソースを作る
	particle3D->vertices_ = ModelManager::GetInstance()->GetModelData(modelHandle).vertices;
	particle3D->vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * particle3D->vertices_.size());



	//リソースの先頭のアドレスから使う
	particle3D->vertexBufferView_.BufferLocation = particle3D->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	particle3D->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * particle3D->vertices_.size());
	//１頂点あたりのサイズ
	particle3D->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	particle3D->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//
	std::memcpy(vertexData, particle3D->vertices_.data(), sizeof(VertexData) * particle3D->vertices_.size());
	particle3D->vertexResource_->Unmap(0, nullptr);



	//インスタンシング
	particle3D->instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);

	//SRVを作る
	particle3D->instancingIndex_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForStructuredBuffer(particle3D->instancingIndex_, particle3D->instancingResource_.Get(), MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));
	//書き込み
	particle3D->instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&particle3D->instancingData_));


	//カメラ
	particle3D->cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Vector3));


	return particle3D;

}




//生成関数
Particle Particle3D::MakeNewParticle(const std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribute(-2.0f, 2.0f);
	Particle particle;
	particle.transform.scale = {.x= 1.0f,.y= 1.0f,.z= 1.0f };
	particle.transform.rotate = {.x= 0.0f,.y= 0.0f,.z= 0.0f };
	//ランダムの値
	Vector3 randomTranslate = {.x= distribute(randomEngine),.y= distribute(randomEngine),.z= distribute(randomEngine) };
	particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, randomTranslate);
	if (moveType_ == ThrowUp) {
		Vector3 offset = { .x = randomTranslate.x,.y = 0.1f,.z = randomTranslate.z };
		particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, offset);

	}

	//速度
	std::uniform_real_distribution<float>distVelocity(-1.0f, 1.0f);
	particle.velocity = {.x= distVelocity(randomEngine),.y= distVelocity(randomEngine),.z= distVelocity(randomEngine) };

	//Color
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = {.x= distColor(randomEngine),.y= distColor(randomEngine),.z= distColor(randomEngine),.w= 1.0f};

	//時間
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;

	return particle;

}


std::list<Particle> Particle3D::Emission(const Emitter& emmitter, std::mt19937& randomEngine) {
	std::list<Particle> particles;

	for (uint32_t count = 0; count < emmitter.count; ++count) {
		//emmitterで設定したカウントまで増やしていくよ
		particles.push_back(MakeNewParticle(randomEngine));
	}

	return particles;
}


void Particle3D::Update(const Camera& camera) {


	//C++でいうsrandみたいなやつ
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	///時間経過
	emitter_.frequencyTime += DELTA_TIME;
	//頻度より大きいなら
	if (emitter_.frequency <= emitter_.frequencyTime) {
		//パーティクルを作る
		particles_.splice(particles_.end(), Emission(emitter_, randomEngine));
		//余計に過ぎた時間も加味して頻度計算する
		emitter_.frequencyTime -= emitter_.frequency;
	}


	//座標の計算など
	numInstance_ = 0;
	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end(); ++particleIterator) {

		//行列の初期化
		Matrix4x4 worldMatrix = {};
		Matrix4x4 scaleMatrix = {};
		Matrix4x4 translateMatrix = {};
		Matrix4x4 billBoardMatrix = {};
		Matrix4x4 backToFrontMatrix = {};



		switch (moveType_) {
		case NormalRelease:
			#pragma region //通常の放出
			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			
				continue;
			}
			//強制的にビルボードにするよ
			particleIterator->transform.translate.x += particleIterator->velocity.x * DELTA_TIME;
			particleIterator->transform.translate.y += particleIterator->velocity.y * DELTA_TIME;
			particleIterator->transform.translate.z += particleIterator->velocity.z * DELTA_TIME;

			//Y軸でπ/2回転
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix_);
			//平行成分はいらないよ
			//あくまで回転だけ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			//行列を作っていくよ
			scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix(particleIterator->transform.scale);
			translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix(particleIterator->transform.translate);


			//パーティクル個別のRotateは関係ないよ
			//その代わりにさっき作ったbillBoardMatrixを入れるよ
			worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

			//最大値を超えて描画しないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				instancingData_[numInstance_].World = worldMatrix;
				instancingData_[numInstance_].color = particleIterator->color;

				//透明になっていくようにするかどうか
				if (isToTransparent_ == true) {
					//アルファはVector4でのwだね
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
					instancingData_[numInstance_].color.w = alpha;

				}

				++numInstance_;
			}

			break;


#pragma endregion


		case ThrowUp:
			#pragma region 鉛直投げ上げ
			//強制的にビルボードにするよ

			//加速
			float accel = -0.001f;
			velocityY_ += accel;

			//加速を踏まえた位置計算
			particleIterator->transform.translate.x += particleIterator->velocity.x / 3.0f;
			particleIterator->transform.translate.y += velocityY_;
			particleIterator->transform.translate.z += particleIterator->velocity.z / 3.0f;

			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix_);
			//平行成分はいらないよ
			//あくまで回転だけ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			//行列を作っていくよ
			scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix(particleIterator->transform.scale);
			translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix(particleIterator->transform.translate);


			//パーティクル個別のRotateは関係ないよ
			//その代わりにさっき作ったbillBoardMatrixを入れるよ
			worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

			//最大値を超えて描画しないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				instancingData_[numInstance_].World = worldMatrix;
				instancingData_[numInstance_].color = particleIterator->color;

				//ワールド座標
				Vector3 worldPosition = {
					.x = worldMatrix.m[3][0],
					.y = worldMatrix.m[3][1],
					.z = worldMatrix.m[3][2]
				};

				//0より小さくなったら透明
				if (worldPosition.y < 0.0f) {
					//アルファはVector4でのwだね
					instancingData_[numInstance_].color.w = 0.0f;

				}

				++numInstance_;
			}
			break;

			#pragma endregion

		case Rise:
#pragma region 上昇
			//強制的にビルボードにするよ
			particleIterator->transform.translate.x += particleIterator->velocity.x / 3.0f;
			particleIterator->transform.translate.y += 0.1f;
			particleIterator->transform.translate.z += particleIterator->velocity.z / 3.0f;

			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix_);
			//平行成分はいらないよ
			//あくまで回転だけ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			//行列を作っていくよ
			scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix(particleIterator->transform.scale);
			translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix(particleIterator->transform.translate);


			//パーティクル個別のRotateは関係ないよ
			//その代わりにさっき作ったbillBoardMatrixを入れるよ
			worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

			//最大値を超えて描画しないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				instancingData_[numInstance_].World = worldMatrix;
				instancingData_[numInstance_].color = particleIterator->color;

				//ワールド座標
				Vector3 worldPosition = {
					.x = worldMatrix.m[3][0],
					.y = worldMatrix.m[3][1],
					.z = worldMatrix.m[3][2]
				};
				//透明になっていくようにするかどうか
				if (isToTransparent_ == true) {
					//アルファはVector4でのwだね
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
					instancingData_[numInstance_].color.w = alpha;

				}

				++numInstance_;
			}
			break;

		}
	}
}

void Particle3D::Draw(const Camera& camera,const  Material& material,const DirectionalLight& directionalLight) {

	//Directionalではなかったらassert
	if (material.lightingKinds_ != Directional) {
		assert(0);
	}

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];

	*cameraPositionData_ = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);


	//コマンドを積む
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());

	//インスタンシング
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2, textureHandle_);
	}

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, camera.bufferResource_->GetGPUVirtualAddress());

	//平行光源
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLight.bufferResource_->GetGPUVirtualAddress());

	//PS用のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0, 0);

}

void Particle3D::Draw(const Camera& camera, const Material& material, const PointLight& pointLight){
	//Directionalではなかったらassert
	if (material.lightingKinds_ != Point) {
		assert(0);
	}

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];

	*cameraPositionData_ = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);


	//コマンドを積む
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());

	//インスタンシング
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2, textureHandle_);
	}

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, camera.bufferResource_->GetGPUVirtualAddress());

	//PS用のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//点光源
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLight.bufferResource_->GetGPUVirtualAddress());

	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0, 0);

}

void Particle3D::Draw(const Camera& camera, const Material& material, const SpotLight& spotLight){
	//Directionalではなかったらassert
	if (material.lightingKinds_ != Spot) {
		assert(0);
	}

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];

	*cameraPositionData_ = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);


	//コマンドを積む
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());

	//インスタンシング
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2, textureHandle_);
	}

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, camera.bufferResource_->GetGPUVirtualAddress());

	//PS用のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());
	
	//SpotLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(7, spotLight.bufferResource_->GetGPUVirtualAddress());


	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0, 0);

}

