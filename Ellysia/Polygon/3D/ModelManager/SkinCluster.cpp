#include "SkinCluster.h"
#include <SrvManager.h>
#include <Matrix4x4Calculation.h>
#include <algorithm>
#include <PipelineManager.h>
#include "ModelManager.h"
#include <cassert>

void  SkinCluster::Create(const Skeleton& skeleton, const ModelData& modelData){
    skeleton_ = skeleton;
    //palette用のResourceを確保
    //作り方は今までと大体同じだね！
    paletteResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WellForGPU) * skeleton_.joints_.size());
    WellForGPU* mappedPalette = nullptr;
    paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
    //spanを使ってアクセスするようにする
    mappedPalette_ = { mappedPalette,skeleton.joints_.size() };
    srvIndex_ = SrvManager::GetInstance()->Allocate();
    paletteSrvHandle_.first = SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex_);
    paletteSrvHandle_.second = SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex_);


    //palette用のSRVを作成
    D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
    paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    paletteSrvDesc.Buffer.FirstElement = 0;
    paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    paletteSrvDesc.Buffer.NumElements = UINT(skeleton_.joints_.size());
    paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

    //SRVを作る
    DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
        paletteResource_.Get(), &paletteSrvDesc, paletteSrvHandle_.first);




    //influence用のResourceを確保
    //頂点ごとにinfluence情報
    influenceResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
    VertexInfluence* mappedInfluence = nullptr;
    influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
    //0埋め。Weightを0にしておく
    std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
    mappedInfluence_ = { mappedInfluence,modelData.vertices.size() };

    //Influence用のVertexBufferViewを作成
    influenceBufferView_.BufferLocation = influenceResource_->GetGPUVirtualAddress();
    influenceBufferView_.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
    influenceBufferView_.StrideInBytes = sizeof(VertexInfluence);

    //InfluenceBindPoseMatrixの保存領域を作成
    inverseBindPoseMatrices.resize(skeleton_.joints_.size());
    //最後の所は「関数ポインタ」
    //()を付けないでね。ここ重要。
    std::generate(inverseBindPoseMatrices.begin(), inverseBindPoseMatrices.end(), MakeIdentity4x4);

#pragma region std::generateについて
    ////std::generate...初期化するときに便利！
    ////for文と似ているのでそっちでやっても◎
    ////実際はこんな感じ
    //for (int i = 0; i < skeleton_.joints.size(); ++i) {
    //    skinCluster.inverseBindPoseMatrices[i] = MakeIdentity4x4();
    //}
#pragma endregion

    //ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める
    for (const auto& jointWeight : modelData.skinClusterData) {
        //JointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
        auto it = skeleton_.jointMap_.find(jointWeight.first);
        if (it == skeleton_.jointMap_.end()) {
            //その名前のJointは存在しないので次に回す
            continue;
        }
        //(*it).secondにはjointのindexが入っているので、街灯のindexのinverseBindPoseMatrixを代入
        inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
        for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
            //該当のvertexIndexのinfluence情報を参照しておく
            auto& currentInfluence = mappedInfluence_[vertexWeight.vertexIndex];
            
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



}

void SkinCluster::Update(){
    for (size_t jointIndex = 0; jointIndex < skeleton_.joints_.size(); ++jointIndex) {
        assert(jointIndex < inverseBindPoseMatrices.size());
        mappedPalette_[jointIndex].skeletonSpaceMatrix =
            Multiply(inverseBindPoseMatrices[jointIndex], skeleton_.joints_[jointIndex].skeletonSpaceMatrix);
        mappedPalette_[jointIndex].skeletonSpaceIncerseTransposeMatrix = MakeTransposeMatrix(
            Inverse(mappedPalette_[jointIndex].skeletonSpaceMatrix));
    }
}
