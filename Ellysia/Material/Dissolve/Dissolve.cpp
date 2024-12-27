#include "Dissolve.h"

void Dissolve::Initialize(){
	//初期化の値を入れる
	//Edgeを使うかどうか
	isUseEdge_=true;
	//Edgeの色
	edgeColor_ = {.x=0.0f,.y=0.0f,.z=0.0f};
	//Edgeの厚さ
	edgeThinkness_=0.04f;
	//閾値
	threshold_=0.5f;
	
	//リソースを生成
	resource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DissolveData));

}

void Dissolve::Update(){
	//書き込み
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&dissolveData_));
	//Edgeを使うかどうか
	dissolveData_->isUseEdge = isUseEdge_;
	//Edgeの色
	dissolveData_->edgeColor = edgeColor_;
	//Edgeの厚さ
	dissolveData_->edgeThinkness = edgeThinkness_;
	//閾値
	dissolveData_->threshold = threshold_;
	//書き込み終了
	resource_->Unmap(0, nullptr);

}
