#include "SkinClushter.h"
#include <SrvManager.h>
#include <Matrix4x4Calculation.h>
#include <algorithm>
#include <PipelineManager.h>

void  SkinCluster::CreateSkinClusher(const Skeleton& skeleton, const ModelData& modelData){
    SkinClusterStruct skinCluster;
    skeleton_ = skeleton;
    //palette用のResourceを確保
    //作り方は今までと大体同じだね！
    skinCluster.paletteResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WellForGPU) * skeleton_.joints.size());
    WellForGPU* mappedPalette = nullptr;
    skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
    //spanを使ってアクセスするようにする
    skinCluster.mappedPalette = { mappedPalette,skeleton_.joints.size() };
    skinCluster.srvIndex = SrvManager::GetInstance()->Allocate();
    skinCluster.paletteSrvHandle.first = SrvManager::GetInstance()->GetCPUDescriptorHandle(skinCluster.srvIndex);
    skinCluster.paletteSrvHandle.second = SrvManager::GetInstance()->GetGPUDescriptorHandle(skinCluster.srvIndex);


    //palette用のSRVを作成
    D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
    paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    paletteSrvDesc.Buffer.FirstElement = 0;
    paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    paletteSrvDesc.Buffer.NumElements = UINT(skeleton_.joints.size());
    paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

    //SRVを作る
    DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
        skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);




    //influence用のResourceを確保
    //頂点ごとにinfluence情報
    skinCluster.influenceResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
    VertexInfluence* mappedInfluence = nullptr;
    skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
    //0埋め。Weightを0にしておく
    std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
    skinCluster.mappedInfluence = { mappedInfluence,modelData.vertices.size() };

    //Influence用のVertexBufferViewを作成
    skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
    skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
    skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

    //InfluenceBindPoseMatrixの保存領域を作成
    skinCluster.inverseBindPoseMatrices.resize(skeleton_.joints.size());
    //std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentity4x4());

#pragma region std::generateについて
    //std::generate...初期化するときに便利！
    //for文と似ているのでそっちでやっても◎
    //実際はこんな感じ
    for (int i = 0; i < skeleton_.joints.size(); ++i) {
        skinCluster.inverseBindPoseMatrices[i] = MakeIdentity4x4();
    }
#pragma endregion

    //ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める
    for (const auto& jointWeight : modelData.skinClusterData) {
        //JointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
        auto it = skeleton_.jointMap.find(jointWeight.first);
        if (it == skeleton_.jointMap.end()) {
            //その名前のJointは存在しないので次に回す
            continue;
        }
        //(*it).secondにはjointのindexが入っているので、街灯のindexのinverseBindPoseMatrixを代入
        skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
        for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
            //該当のvertexIndexのinfluence情報を参照しておく
            auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
            
            //空いている所に入れる
            for (uint32_t index = 0; index < NUM_MAX_INFLUENCE; ++index) {
                //weight==0が空いている状態なので、その場所にweightとjointのindexを代入
                if (currentInfluence.weights[index] == 0.0f) {
                    currentInfluence.weights[index] = vertexWeight.weight;
                    currentInfluence.jointIndices[index] = (*it).second;
                    break;
                }

            }
        }


    }



    skinClusterStruct_= skinCluster;
}

void SkinCluster::Update(){

    for (size_t jointIndex = 0; jointIndex < skeleton_.joints.size(); ++jointIndex) {
        assert(jointIndex < skinClusterStruct_.inverseBindPoseMatrices.size());
        skinClusterStruct_.mappedPalette[jointIndex].skeletonSpaceMatrix =
            Multiply(skinClusterStruct_.inverseBindPoseMatrices[jointIndex], skeleton_.joints[jointIndex].skeletonSpaceMatrix);
        skinClusterStruct_.mappedPalette[jointIndex].skeletonSpaceIncerseTransposeMatrix = MakeTransposeMatrix(
            Inverse(skinClusterStruct_.mappedPalette[jointIndex].skeletonSpaceMatrix));
    }
}

void SkinCluster::Draw(WorldTransform& worldTransform, Camera& camera){

    //Skinning
    DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetSkinningRootSignature().Get());
    DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());

    //WorldTransform
    DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldTransform.bufferResource_->GetGPUVirtualAddress());


    //Camera
    DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, camera.bufferResource_->GetGPUVirtualAddress());

    
    //インスタンシングから
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, skinClusterStruct_.srvIndex);

}
