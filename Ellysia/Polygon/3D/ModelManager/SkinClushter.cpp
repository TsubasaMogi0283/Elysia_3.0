#include "SkinClushter.h"
#include <SrvManager.h>

SkinClushter CreateSkinClusher(const Skeleton& skeleton, const ModelData& modelData, 
    ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize){
    SkinClushter skinCluster;
    
    //palette用のResourceを確保
    //作り方は今までと大体同じだね！
    skinCluster.paletteResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
    WellForGPU* mappedPalette = nullptr;
    skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
    //spanを使ってアクセスするようにする
    skinCluster.mappedPalette = { mappedPalette,skeleton.joints.size() };
    uint32_t index = SrvManager::GetInstance()->Allocate();

    skinCluster.paletteSrvHandle.first = SrvManager::GetInstance()->GetCPUDescriptorHandle(index);
    skinCluster.paletteSrvHandle.second = SrvManager::GetInstance()->GetGPUDescriptorHandle(index);


    //palette用のSRVを作成
    D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
    paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    paletteSrvDesc.Buffer.FirstElement = 0;
    paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
    paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

    //SRVを作る
    DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
        skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);



    //influence用のResourceを確保
    //頂点ごとにinfluence情報
    

    //Influence用のVertexBufferViewを作成
    //InfluenceBindPoseMatrixの保存領域を作成
    //ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める




    return skinCluster;
}
