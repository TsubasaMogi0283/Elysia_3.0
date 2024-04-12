#include "Transformation.h"
#include <Matrix4x4.h>
#include <Matrix4x4Calculation.h>
#include <WindowsSetup.h>
#include "Camera.h"



void Transformation::Initialize(){
	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix)).Get();
}

void Transformation::SetInformation(Transform transform){
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));


	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
	//遠視投影行列
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 view = MakeIdentity4x4();
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(view, view));


	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World =MakeIdentity4x4();

}

void Transformation::SetGraphicCommand(){
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
}
