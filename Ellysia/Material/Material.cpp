#include "Material.h"
#include <Matrix4x4Calculation.h>

void Material::Initialize(){
	//色
	color_ = {.x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f};
	//ライティングの種類
	lightingKinds_ = DirectionalLighting;
	//UV行列
	uvTransform_= Matrix4x4Calculation::MakeIdentity4x4();
	//輝度
	//かなり大きくしないと変になる。
	shininess_=1000.0f;
	//環境マップ
	isEnviromentMap_ = false;
	//リソースを生成
	bufferResource_ = Ellysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(MaterialData));

}

void Material::Update(){

	//書き込み
	bufferResource_->Map(0u, nullptr, reinterpret_cast<void**>(&materialData_));
	//色
	materialData_->color = color_;
	//ライティングの種類
	materialData_->lightingKinds = lightingKinds_;
	//UVトランスフォーム
	materialData_->uvTransform = uvTransform_;
	//輝度
	materialData_->shininess = shininess_;
	//環境マップ
	materialData_->isEnviromentMap = isEnviromentMap_;
	//書き込み終了
	bufferResource_->Unmap(0u, nullptr);
}
