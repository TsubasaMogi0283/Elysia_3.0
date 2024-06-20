#include "Material.h"
#include <Matrix4x4Calculation.h>

void Material::Initialize(){
	//色
	color_ = {1.0f,1.0f,1.0f,1.0f};
	//Lightingの種類
	lightingKinds_ = Directional;
	//UV行列
	uvTransform_= MakeIdentity4x4();
	//輝度
	//かなり大きくしないと変になる。
	shininess_=1000.0f;

	//定数バッファ
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(MaterialData));

}

void Material::Update(){
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	materialData_->lightingKinds = lightingKinds_;
	materialData_->uvTransform = uvTransform_;
	materialData_->shininess = shininess_;
	bufferResource_->Unmap(0, nullptr);
}
