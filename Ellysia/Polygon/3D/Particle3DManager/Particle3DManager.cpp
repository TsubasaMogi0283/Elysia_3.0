#include "Particle3DManager.h"



#include <vector>


#include "DirectXSetup.h"
#include "PipelineManager.h"

#include "ModelManager.h"
#include "SrvManager.h"
#include "CollisionCalculation.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include <numbers>
#include "TextureManager.h"
#include "Material.h"

Particle3DManager* Particle3DManager::GetInstance(){
    static Particle3DManager instance;
    return &instance;
}

void Particle3DManager::Initialize(){


	std::mt19937 randomEngine(seedGenerator());
	rand_ = randomEngine;
    //パイプラインの生成
    PipelineManager::GetInstance()->GenerateParticle3DPSO();
     


    
    //頂点データの初期化
    //頂点リソースの生成
    //頂点バッファビュー作成
    //頂点リソースに頂点データを書き込む

	uint32_t particleModel = ModelManager::GetInstance()->LoadModelFile("Resources/05_02", "plane.obj");
    vertices_ = ModelManager::GetInstance()->GetModelData(particleModel).vertices;

    
    vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * vertices_.size()).Get();
	materialResource_= DirectXSetup::GetInstance()->CreateBufferResource(sizeof(MaterialData)).Get();

    //読み込みのところでバッファインデックスを作った方がよさそう
    //vertexResourceがnullらしい
    //リソースの先頭のアドレスから使う
   
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースは頂点のサイズ
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size());
    //１頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);



	directionalLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLightData)).Get();


#pragma region 頂点データ
    //頂点バッファにデータを書き込む
    VertexData* vertexData = nullptr;
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
    std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());
    vertexResource_->Unmap(0, nullptr);

#pragma endregion


	accelerationField_.acceleration = { 5.0f,0.0f,0.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };



}

void Particle3DManager::CreateParticleGroup(const std::string name, uint32_t textureHandle, Vector3 position, uint32_t count){
    

    //空のグループを生成
    ParticleGrounp particles;

    //マテリアルデータにテクスチャグループを作成しする
   
    //マテリアルデータにテクスチャのSRVインデックスを記録
    //テクスチャの読み込みはTextureManagerでやっているよ
	particles.textureSrvindex = textureHandle;
    

    //インスタンス用のリソースを生成
    particles.instancingResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);
    
    

    //インスタンシング用にSRVを確保してSRVインデックスを記録
    particles.instancingSrvIndex = SrvManager::GetInstance()->Allocate();
    //SRV生成
    SrvManager::GetInstance()->CreateSRVForStructuredBuffer(
		particles.instancingSrvIndex, particles.instancingResource.Get(), MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));


	particles.emitter_.frequency = 1.0f;
	particles.emitter_.frequencyTime = 0.0f;
	particles.emitter_.count = count;
	particles.emitter_.transform.scale = {1.0f,1.0f,1.0f};
	particles.emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	particles.emitter_.transform.translate = position;

    //登録
    particleGroup_[name] = particles;



}


//生成関数
Particle Particle3DManager::MakeNewParticle(std::mt19937& randomEngine, Vector3 position) {
	std::uniform_real_distribution<float> distribute(-1.0f, 1.0f);
	Particle particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	//ランダムの値
	Vector3 randomTranslate = {distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};
	particle.transform.translate = VectorCalculation::Add(position, randomTranslate);

	//速度
	std::uniform_real_distribution<float>distVelocity(-1.0f, 1.0f);
	particle.velocity = { distVelocity(randomEngine),distVelocity(randomEngine),distVelocity(randomEngine) };

	//Color
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };


	//時間
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;



	return particle;

}

//エミッタ
std::list<Particle> Particle3DManager::Emission(Vector3 position,uint32_t count, std::mt19937& randomEngine) {
	std::list<Particle> particles;

	for (uint32_t i = 0; i < count; ++i) {
		particles.push_back(MakeNewParticle(randomEngine, position));
	}

	return particles;
}

//作り方が分からない
void Particle3DManager::Emit(const std::string name, uint32_t textureHandle, const Vector3& position, uint32_t count) {
	//とりあえず登録用にしておく
	CreateParticleGroup(name, textureHandle,position,count);
}


//更新
void Particle3DManager::Update(Camera& camera) {

	//二重for文はこういうことだろうか
	for (auto& particleGroupPair : particleGroup_) {
		//secondで中身にアクセス
		ParticleGrounp& particleGroup = particleGroupPair.second;

		



		///時間経過
		particleGroup.emitter_.frequencyTime += DELTA_TIME;
		//頻度より大きいなら
		if (particleGroup.emitter_.frequency <= particleGroup.emitter_.frequencyTime) {

			particleGroup.particles.splice(particleGroup.particles.end(), Emission(particleGroup.emitter_.transform.translate, particleGroup.emitter_.count, rand_));

			//余計に過ぎた時間も加味して頻度計算する
			particleGroup.emitter_.frequencyTime -= particleGroup.emitter_.frequency;
		}

		//座標の計算など
		particleGroup.instanceNumber = 0;
		for (std::list<Particle>::iterator particleIterator = particleGroup.particles.begin();
			particleIterator != particleGroup.particles.end(); ++particleIterator) {

			


			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {

				continue;
			}
			//フィールド設定すると風の影響を受ける
			if (isSetField_ == true) {

				if (CollisionCalculation::IsCollisionAABBAndPoint(accelerationField_.area, (*particleIterator).transform.translate)) {
					(*particleIterator).velocity.x += accelerationField_.acceleration.x * DELTA_TIME;
					(*particleIterator).velocity.y += accelerationField_.acceleration.y * DELTA_TIME;
					(*particleIterator).velocity.z += accelerationField_.acceleration.z * DELTA_TIME;
				}
			}


			(*particleIterator).currentTime += DELTA_TIME;
			(*particleIterator).transform.translate.x += (*particleIterator).velocity.x * DELTA_TIME;
			(*particleIterator).transform.translate.y += (*particleIterator).velocity.y * DELTA_TIME;
			(*particleIterator).transform.translate.z += (*particleIterator).velocity.z * DELTA_TIME;


			//ビルボード有り
			if (isBillBordMode_ == true) {
				//Y軸でπ/2回転
				//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
				Matrix4x4 backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

				//カメラの回転を適用する
				Matrix4x4 billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix_);
				//平行成分はいらないよ
				billBoardMatrix.m[3][0] = 0.0f;
				billBoardMatrix.m[3][1] = 0.0f;
				billBoardMatrix.m[3][2] = 0.0f;

				Matrix4x4 scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix((*particleIterator).transform.scale);
				Matrix4x4 translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix((*particleIterator).transform.translate);


				//パーティクル個別のRotateは関係ないよ
				Matrix4x4 worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

				//最大値を超えて描画しないようにする
				if (particleGroup.instanceNumber < MAX_INSTANCE_NUMBER_) {
					particleGroup.instancingData[particleGroup.instanceNumber].world = worldMatrix;
					particleGroup.instancingData[particleGroup.instanceNumber].color = (*particleIterator).color;

					//アルファはVector4でいうwだね
					float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
					particleGroup.instancingData[particleGroup.instanceNumber].color.w = alpha;

					++particleGroup.instanceNumber;
				}
			}
			//ビルボード無し
			else if (isBillBordMode_ == false) {
				//ビルボードやらない版
				Matrix4x4 worldMatrix = Matrix4x4Calculation::MakeAffineMatrix(
					(*particleIterator).transform.scale,
					(*particleIterator).transform.rotate,
					(*particleIterator).transform.translate);

				//WVP行列を作成

				//最大値を超えて描画しないようにする
				if (particleGroup.instanceNumber < MAX_INSTANCE_NUMBER_) {
					particleGroup.instancingData[particleGroup.instanceNumber].world = worldMatrix;
					particleGroup.instancingData[particleGroup.instanceNumber].color = (*particleIterator).color;

					//アルファはVector4でいうwだね
					float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
					particleGroup.instancingData[particleGroup.instanceNumber].color.w = alpha;

					++particleGroup.instanceNumber;
				}
			}


		}
	}

}




void Particle3DManager::Draw(Camera& camera) {
	//更新
	Update(camera);





	for (auto& particleGroupPair : particleGroup_) {
		//secondで中身にアクセス
		ParticleGrounp& particleGroup = particleGroupPair.second;
		

#pragma region マテリアルにデータを書き込む
		//書き込むためのアドレスを取得
		//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
		MaterialData* materialData_ = nullptr;
		materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
		materialData_->color = materialColor_;
		materialData_->lightingKinds = isEnableLighting_;

		materialData_->uvTransform = Matrix4x4Calculation::MakeIdentity4x4();

		materialResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region DirectionalLight
		DirectionalLightData* directionalLightData_ = nullptr;
		directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
		directionalLightData_->color = directionalLightColor_;
		directionalLightData_->direction = lightingDirection_;
		directionalLightData_->intensity = directionalLightIntensity_;
		directionalLightResource_->Unmap(0, nullptr);

#pragma endregion

		//コマンドを積む
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
		DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());



		//RootSignatureを設定。PSOに設定しているけど別途設定が必要
		DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
		//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
		DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//CBVを設定する
		//マテリアル
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

		//インスタンシング
		particleGroup.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instancingData));
		SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, particleGroup.instancingSrvIndex);


		//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
		if (particleGroup.textureSrvindex != 0) {
			TextureManager::GraphicsCommand(2,particleGroup.textureSrvindex);
		}

		//DirectionalLight
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

		//カメラ
		DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());




		//DrawCall
		DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), particleGroup.instanceNumber, 0, 0);
	}


}

void Particle3DManager::DeleteElement(const std::string name){
	particleGroup_.erase(name);
}
