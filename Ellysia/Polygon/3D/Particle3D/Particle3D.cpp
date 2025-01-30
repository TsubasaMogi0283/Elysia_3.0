#include "Particle3D.h" 

#include <cassert>
#include <numbers>
#include <algorithm>

#include "Camera.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "ModelManager.h"

#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "VectorCalculation.h"
#include "SrvManager.h"


Particle3D::Particle3D() {
	//モデル管理クラスの取得
	modelManager_ = Ellysia::ModelManager::GetInstance();

	//テクスチャ管理クラスの取得
	textureManager_ = Ellysia::TextureManager::GetInstance();

	//DirectXクラスの取得
	directXSetup_ = Ellysia::DirectXSetup::GetInstance();

	//SRV管理クラスの取得
	srvManager_ = Ellysia::SrvManager::GetInstance();

	//パイプライン管理クラスの取得
	pipelineManager_ = Ellysia::PipelineManager::GetInstance();

}

Particle3D* Particle3D::Create(const uint32_t& moveType){
	//生成
	Particle3D* particle3D = new Particle3D();

#pragma region デフォルトの設定 
	particle3D->emitter_.count = 10;
	//0.5秒ごとに発生
	particle3D->emitter_.frequency = 0.0f;
	//発生頻度用の時刻。0.0で初期化
	particle3D->emitter_.frequencyTime = 0.0f;
	//SRT
	particle3D->emitter_.transform.scale = {.x = 20.0f,.y = 20.0f,.z = 20.0f };
	particle3D->emitter_.transform.rotate = { .x = 0.0f,.y = 0.0f,.z = 0.0f };
	particle3D->emitter_.transform.translate = { .x = 0.0f,.y = 0.0f,.z = 4.0f };

	//モデルの読み込み
	uint32_t modelHandle = particle3D->modelManager_->LoadModelFile("Resources/Model/Particle","ParticlePlane.obj");

	//テクスチャの読み込み
	particle3D->textureHandle_ = particle3D->textureManager_->LoadTexture("Resources/External/Texture/Circle/circle.png");

#pragma endregion


	//動きの種類
	particle3D->moveType_ = moveType;

	//頂点リソースを作る
	particle3D->vertices_ = particle3D->modelManager_->GetModelData(modelHandle).vertices;
	particle3D->vertices_ = particle3D->modelManager_->GetModelData(modelHandle).vertices;
	particle3D->vertexResource_ = particle3D->directXSetup_->CreateBufferResource(sizeof(VertexData) * particle3D->vertices_.size());


	//VBVとか関数でまとめたい

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
	particle3D->instancingResource_ = particle3D->directXSetup_->CreateBufferResource(sizeof(ParticleForGPU) * particle3D->MAX_INSTANCE_NUMBER_);

	//SRVを作る
	particle3D->instancingIndex_ = particle3D->srvManager_->Allocate();
	particle3D->srvManager_->CreateSRVForStructuredBuffer(particle3D->instancingIndex_, particle3D->instancingResource_.Get(), particle3D->MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));
	//書き込み
	particle3D->instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&particle3D->instancingData_));

	//カメラ
	particle3D->cameraResource_ = particle3D->directXSetup_->CreateBufferResource(sizeof(Vector3));


	return particle3D;

}

Particle3D* Particle3D::Create(const uint32_t& modelHandle,const uint32_t& moveType) {
	Particle3D* particle3D = new Particle3D();

#pragma region デフォルトの設定 
	particle3D->emitter_.count = 3;
	//0.5秒ごとに発生
	particle3D->emitter_.frequency = 2.0f;
	//発生頻度用の時刻。0.0で初期化
	particle3D->emitter_.frequencyTime = 0.0f;
	//SRT
	particle3D->emitter_.transform.scale = {.x= 10.0f,.y= 10.0f,.z= 10.0f };
	particle3D->emitter_.transform.rotate = {.x= 0.0f,.y= 0.0f,.z= 0.0f };
	particle3D->emitter_.transform.translate = {.x= 0.0f,.y= 0.0f,.z= 4.0f };

#pragma endregion

	//モデルの読み込み
	ModelData modelData = particle3D->modelManager_->GetModelData(modelHandle);

	//テクスチャの読み込み
	particle3D->textureHandle_ = particle3D->textureManager_->LoadTexture(modelData.textureFilePath);

	//動きの種類
	particle3D->moveType_ = moveType;

	//頂点リソースを作る
	particle3D->vertices_ = particle3D->modelManager_->GetModelData(modelHandle).vertices;
	particle3D->vertexResource_ = particle3D->directXSetup_->CreateBufferResource(sizeof(VertexData) * particle3D->vertices_.size());



	//リソースの先頭のアドレスから使う
	particle3D->vertexBufferView_.BufferLocation = particle3D->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	particle3D->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * particle3D->vertices_.size());
	//１頂点あたりのサイズ
	particle3D->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	particle3D->vertexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vertexData));//
	std::memcpy(vertexData, particle3D->vertices_.data(), sizeof(VertexData) * particle3D->vertices_.size());
	particle3D->vertexResource_->Unmap(0u, nullptr);



	//インスタンシング
	particle3D->instancingResource_ = particle3D->directXSetup_->CreateBufferResource(sizeof(ParticleForGPU) * particle3D->MAX_INSTANCE_NUMBER_);

	//SRVを作る
	particle3D->instancingIndex_ = particle3D->srvManager_->Allocate();
	particle3D->srvManager_->CreateSRVForStructuredBuffer(particle3D->instancingIndex_, particle3D->instancingResource_.Get(), particle3D->MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));
	//書き込み
	particle3D->instancingResource_->Map(0u, nullptr, reinterpret_cast<void**>(&particle3D->instancingData_));


	//カメラ
	particle3D->cameraResource_ = particle3D->directXSetup_->CreateBufferResource(sizeof(Vector3));


	return particle3D;

}

Particle Particle3D::MakeNewParticle(std::mt19937& randomEngine) {

	//ランダムの値で位置を決める
	//SRは固定
	std::uniform_real_distribution<float> distribute(-2.0f, 2.0f);
	Particle particle;
	particle.transform.scale = {.x= 1.0f,.y= 1.0f,.z= 1.0f };
	particle.transform.rotate = {.x= 0.0f,.y= 0.0f,.z= 0.0f };
	Vector3 randomTranslate = {.x= distribute(randomEngine),.y= distribute(randomEngine)+1.0f,.z= distribute(randomEngine) };
	particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, randomTranslate);
	//投げ上げは少しだけ上にずらす
	if (moveType_ == ThrowUp) {
		Vector3 offset = { .x = randomTranslate.x,.y = 0.1f,.z = randomTranslate.z };
		particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, offset);

	}

	//速度
	std::uniform_real_distribution<float>distVelocity(-1.0f, 1.0f);
	particle.velocity = {.x= distVelocity(randomEngine),.y= distVelocity(randomEngine),.z= distVelocity(randomEngine) };

	//色
	std::uniform_real_distribution<float> distColor(1.0f, 1.0f);
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


	//一度だけ出すモード
	if (isReleaseOnceMode_ == true && isReeasedOnce_ == false) {
		//パーティクルを作る
		particles_.splice(particles_.end(), Emission(emitter_, randomEngine));
		isReeasedOnce_ = true;
	}


	if (isReleaseOnceMode_ == false) {
		//時間経過
		emitter_.frequencyTime += DELTA_TIME;
		//頻度より大きいなら
		if (emitter_.frequency <= emitter_.frequencyTime) {
			//パーティクルを作る
			particles_.splice(particles_.end(), Emission(emitter_, randomEngine));
			//余計に過ぎた時間も加味して頻度計算する
			emitter_.frequencyTime -= emitter_.frequency;
		}
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

		//加速
		float accel = -0.001f;
		particleIterator->currentTime += DELTA_TIME;

		switch (moveType_) {
		case NormalRelease:
			#pragma region 通常の放出

			if (isReleaseOnceMode_ == false) {
				if (particleIterator->lifeTime <= particleIterator->currentTime) {

					continue;
				}
			}

			
			particleIterator->transform.translate.y += 0.0001f;
			
			//Y軸でπ/2回転
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix);
			//平行成分はいらないよ
			//あくまで回転だけ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			//行列を作っていくよ
			//Scale
			scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix(particleIterator->transform.scale);
			//座標
			translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix(particleIterator->transform.translate);


			//パーティクル個別のRotateは関係ないよ
			//その代わりにさっき作ったbillBoardMatrixを入れるよ
			worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

			//最大値を超えて描画しないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				//ワールド行列と色のデータを設定
				instancingData_[numInstance_].world = worldMatrix;
				const Vector4 COLOR = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };
				instancingData_[numInstance_].color = COLOR;
				particleIterator->color = COLOR;

				//透明になっていくようにするかどうか
				if (isToTransparent_ == true) {
					//アルファはVector4でのwだね
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
					instancingData_[numInstance_].color.w = alpha ;
					particleIterator->color.w = alpha;
				}

				++numInstance_;
			}

			break;


#pragma endregion

		case ThrowUp:
			#pragma region 鉛直投げ上げ
			//強制的にビルボードにするよ

			
			velocityY_ += accel;

			//加速を踏まえた位置計算
			particleIterator->transform.translate.x += particleIterator->velocity.x / 3.0f;
			particleIterator->transform.translate.y += velocityY_;
			particleIterator->transform.translate.z += particleIterator->velocity.z / 3.0f;

			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix);
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
				instancingData_[numInstance_].world = worldMatrix;
				instancingData_[numInstance_].color = particleIterator->color;

				//ワールド座標
				Vector3 worldPosition = {
					.x = worldMatrix.m[3][0],
					.y = worldMatrix.m[3][1],
					.z = worldMatrix.m[3][2]
				};

				//設定した地面の高さより小さくなったら透明
				if (worldPosition.y < groundOffset_) {
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
			particleIterator->transform.translate.x += particleIterator->velocity.x / 15.0f;
			particleIterator->transform.translate.y += 0.03f;
			particleIterator->transform.translate.z += particleIterator->velocity.z / 15.0f;
			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix);
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

			//最大値を超えないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				instancingData_[numInstance_].world = worldMatrix;
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
					particleIterator->color.w = alpha;
					instancingData_[numInstance_].color.w = alpha;
				}

				++numInstance_;
			}
			break;

			#pragma endregion

		}

		//見えなくなった
		if (particleIterator->color.w <= 0.0f) {
			particleIterator->isInvisible = true;
		}
		
	}


	//全て見えなくなったらisAllInvisible_がtrueになる
	if (isReeasedOnce_ == true) {
		
		//all_ofは中にある全ての要素が満たす時にtrueを返す
		//今回の場合はparticles_にあるisInvisibleが全てtrueに鳴ったらtrueを返すという仕組みになっている
		isAllInvisible_ = std::all_of(particles_.begin(), particles_.end(), [](const Particle& particle) {
			return particle.isInvisible == true;
		});
		


#ifdef _DEBUG
		ImGui::Begin("パーティクル");
		ImGui::Checkbox("全て消えたか", &isAllInvisible_);
		ImGui::End();
#endif // _DEBUG
	}
}

void Particle3D::Draw(const Camera& camera,const Material& material){

	assert(material.lightingKinds_ == NoneLighting);

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0u, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	*cameraPositionData_ = camera.GetWorldPosition();
	cameraResource_->Unmap(0u, nullptr);


	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetParticle3DRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, material.resource_->GetGPUVirtualAddress());

	//インスタンシング
	srvManager_->SetGraphicsRootDescriptorTable(1u, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0u) {
		textureManager_->GraphicsCommand(2u, textureHandle_);
	}

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3u, camera.bufferResource->GetGPUVirtualAddress());


	//PS用のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5u, cameraResource_->GetGPUVirtualAddress());

	//DrawCall
	directXSetup_->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0u, 0u);


}

void Particle3D::Draw(const Camera& camera,const  Material& material,const DirectionalLight& directionalLight) {

	//Directionalではなかったらassert
	if (material.lightingKinds_ != DirectionalLighting) {
		assert(0);
	}

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0u, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	*cameraPositionData_ = camera.GetWorldPosition();
	cameraResource_->Unmap(0u, nullptr);


	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetParticle3DRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, material.resource_->GetGPUVirtualAddress());

	//インスタンシング
	srvManager_->SetGraphicsRootDescriptorTable(1u, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0u) {
		textureManager_->GraphicsCommand(2u, textureHandle_);
	}

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3u, camera.bufferResource->GetGPUVirtualAddress());

	//平行光源
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(4u, directionalLight.resource->GetGPUVirtualAddress());

	//PS用のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5u, cameraResource_->GetGPUVirtualAddress());

	//DrawCall
	directXSetup_->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0u, 0u);

}

void Particle3D::Draw(const Camera& camera, const Material& material, const PointLight& pointLight){
	//Pointではなかったらassert
	if (material.lightingKinds_ != PointLighting) {
		assert(0);
	}

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0u, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	*cameraPositionData_ = camera.GetWorldPosition();
	cameraResource_->Unmap(0u, nullptr);


	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetParticle3DRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetParticle3DGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, material.resource_->GetGPUVirtualAddress());

	//インスタンシング
	srvManager_->SetGraphicsRootDescriptorTable(1u, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0u) {
		textureManager_->GraphicsCommand(2u, textureHandle_);
	}

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3u, camera.bufferResource->GetGPUVirtualAddress());

	//PS用のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5u, cameraResource_->GetGPUVirtualAddress());

	//点光源
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(6u, pointLight.bufferResource_->GetGPUVirtualAddress());

	//DrawCall
	directXSetup_->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0u, 0u);

}

void Particle3D::Draw(const Camera& camera, const Material& material, const SpotLight& spotLight){
	//Spotではなかったらassert
	if (material.lightingKinds_ != SpotLighting) {
		assert(0);
	}

	//更新
	Update(camera);

	//PS用のカメラ
	cameraResource_->Map(0u, nullptr, reinterpret_cast<void**>(&cameraPositionData_));
	*cameraPositionData_ = camera.GetWorldPosition();
	cameraResource_->Unmap(0u, nullptr);


	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetParticle3DRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, material.resource_->GetGPUVirtualAddress());

	//インスタンシング
	srvManager_->SetGraphicsRootDescriptorTable(1u, instancingIndex_);

	//テクスチャ
	if (textureHandle_ != 0u) {
		textureManager_->GraphicsCommand(2u, textureHandle_);
	}

	//カメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3u, camera.bufferResource->GetGPUVirtualAddress());

	//PS用のカメラ
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(5u, cameraResource_->GetGPUVirtualAddress());
	
	//SpotLight
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(7u, spotLight.bufferResource_->GetGPUVirtualAddress());


	//DrawCall
	directXSetup_->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0u, 0u);

}

