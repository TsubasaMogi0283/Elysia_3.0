#include "SrvManager.h"

const uint32_t SrvManager::MAX_SRV_COUNT_ = 512;


SrvManager* SrvManager::GetInstance(){
	static SrvManager instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
void SrvManager::Initialize(){
	//デスクリプタヒープの生成
	SrvManager::GetInstance()->descriptorHeap_ = DirectXSetup::GetInstance()->GenarateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, MAX_SRV_COUNT_, true);

	//SRV...ShaderResourceView
	//デスクリプタ1個分のサイズを取得して記録
	SrvManager::GetInstance()->descriptorSize_ = DirectXSetup::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


}

uint32_t SrvManager::Allocate(){

	//上限だったらasset
	assert(useIndex_ < MAX_SRV_COUNT_);

	//return する番号を一旦記録しておく
	int index = useIndex_;

	//次のために番号を1進める
	useIndex_++;
	
	//上で記録した番号をreturn
	
	return index;;
}






D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}


void SrvManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2Dテクスチャ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(mipLevels);


	//SRVを作る
	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride){
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));

}

void SrvManager::CreateSRVForRenderTexture(ID3D12Resource* pResource, uint32_t handle){
	//SRVの設定
	//FormatはResourceと同じにしておく
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//SRVの生成
	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(handle));



}

void SrvManager::CreateSRVForDepthTexture(uint32_t handle){
	D3D12_SHADER_RESOURCE_VIEW_DESC depthTextureSrvDesc{};
	//DXGI_FORMAT_D24_UNIFORM_S8_UNITのDepthを読むときはDXGI_FORMAT_R24_UNIFORM_X8_TYPELESS
	depthTextureSrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depthTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	depthTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	depthTextureSrvDesc.Texture2D.MipLevels = 1;
	ComPtr<ID3D12Resource> resource = DirectXSetup::GetInstance()->GerDepthStencilResource();
	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &depthTextureSrvDesc, GetCPUDescriptorHandle(handle));


}

void SrvManager::PreDraw() {
	////コマンドを積む
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	DirectXSetup::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

}



void SrvManager::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex){
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(
		rootParameterIndex,
		GetGPUDescriptorHandle(srvIndex));
}
