#include "Sprite.h"
#include <vector>

#include "TextureManager.h"
#include "PipelineManager.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"

Ellysia::Sprite::Sprite() {
	//DirectXクラスを取得
	directXSetup_ = Ellysia::DirectXSetup::GetInstance();

	//パイプライン管理クラスを取得
	pipelineManager_ = Ellysia::PipelineManager::GetInstance();

}


void Ellysia::Sprite::Initialize(const uint32_t& textureHandle, const Vector2& position) {
	this->textureHandle_ = textureHandle;
	this->position_ = position;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	
	//テクスチャの情報を取得
	resourceDesc_ = Ellysia::TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
	size_ = { float(resourceDesc_.Width),float(resourceDesc_.Height) };


	

	//頂点リソースを作る
	vertexResource_ = directXSetup_->CreateBufferResource(sizeof(VertexData) * 6).Get();
	//index用のリソースを作る
	indexResource_ = directXSetup_->CreateBufferResource(sizeof(uint32_t) * 6).Get();
	

	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//Indexを利用
	//リsp－スの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;



	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = Ellysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(MaterialData));
	//TransformationMatrix用のリソースを作る。
	transformationMatrixResource_ = Ellysia::DirectXSetup::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix)).Get();


	//UVトランスフォームの初期化
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f} 
	};


	
}



Ellysia::Sprite* Ellysia::Sprite::Create(const uint32_t& textureHandle, const Vector2& position) {
	//生成
	Ellysia::Sprite* sprite = new Ellysia::Sprite();
	


	//初期化
	sprite->Initialize(textureHandle,position);

	return sprite;

}
//描画
void Ellysia::Sprite::Draw() {


	//非表示にするかどうか
	if (isInvisible_ == true) {
		return;
	}


	//完全に透明になったら通す必要が無いよね
	if (color_.w <= 0.0f) {
		return;
	}
	

	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//今回は「Sprite」なので三角形が2枚必要
	//-------------
	//|\		  |
	//|  \		  |
	//|    \	  |
	//|      \    |
	//|         \ |
	//-------------

	//13
	//02




	//書き込むためのアドレスを取得
	vertexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vertexData_));

	float left = (0.0f-anchorPoint_.x) * size_.x;
	float right = (1.0f-anchorPoint_.x) * size_.x;
	float top = (0.0f-anchorPoint_.y) * size_.y;
	float bottom = (1.0f-anchorPoint_.y) * size_.y;


	float texLeft =0.0f;
	float texRight = 1.0f;
	float texTop= 0.0f;
	float texBottom= 1.0f;

	//UVをいじりたいとき設定するもの
	if (isUVSetting_ == true) {
		//uv
		texLeft = textureLeftTop_.x / resourceDesc_.Width;
		texRight = (textureLeftTop_.x+textureSize_.x) / resourceDesc_.Width;
		texTop= textureLeftTop_.y / resourceDesc_.Height;
		texBottom= (textureLeftTop_.y +textureSize_.y)/ resourceDesc_.Height;
	}
	



	//左右反転
	if (isFlipX_ == true) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_ == true) {
		top = -top;
		bottom = -bottom;
	}


	//1枚目の三角形
	//左下
	vertexData_[LeftBottom].position = {left,bottom,0.0f,1.0f};
	vertexData_[LeftBottom].texCoord = { texLeft,texBottom };

	//左上
	vertexData_[LeftTop].position = {left,top,0.0f,1.0f};
	vertexData_[LeftTop].texCoord = { texLeft,texTop };
	
	//右下
	vertexData_[RightBottom].position = {right,bottom,0.0f,1.0f} ;
	vertexData_[RightBottom].texCoord = { texRight,texBottom };


	//右上
	vertexData_[RightTop].position = { right,top,0.0f,1.0f };
	vertexData_[RightTop].texCoord = { texRight,texTop };


	//IndexResourceにデータを書き込む
	//インデックスデータにデータを書き込む
	indexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0u;
	indexData_[1] = 1u;
	indexData_[2] = 2u;
	indexData_[3] = 1u;
	indexData_[4] = 3u;
	indexData_[5] = 2u;



	//トランスフォームデータに書き込み
	transformationMatrixResource_->Map(0u, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	
	//座標の再設定
	Vector3 newPosition = {};
	if (isBack_ == true) {
		newPosition = { .x = position_.x,.y = position_.y,.z = 1.0f };
	}
	else {
		newPosition = { .x = position_.x,.y = position_.y,.z = 0.0f };
	}


	//アフィン行列を計算
	Matrix4x4 affineMatrix = Matrix4x4Calculation::MakeAffineMatrix({ scale_.x,scale_.y,1.0f }, { 0.0f,0.0f,rotate_ }, newPosition);
	//遠視投影行列を計算
	Matrix4x4 viewMatrix = Matrix4x4Calculation::MakeIdentity4x4();
	//プロジェクション行列を計算
	Matrix4x4 projectionMatrix = Matrix4x4Calculation::MakeOrthographicMatrix(0.0f, 0.0f, float(Ellysia::WindowsSetup::GetInstance()->GetClientWidth()), float(Ellysia::WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSprite = Matrix4x4Calculation::Multiply(affineMatrix, Matrix4x4Calculation::Multiply(viewMatrix, projectionMatrix));

	transformationMatrixData_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData_->World = Matrix4x4Calculation::MakeIdentity4x4();

	transformationMatrixResource_->Unmap(0u, nullptr);


	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	//ライティングしない
	materialData_->lightingKinds = 0u;
	materialData_->shininess = 0.0f;

	Matrix4x4 uvTransformMatrix = Matrix4x4Calculation::MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix = Matrix4x4Calculation::Multiply(uvTransformMatrix, Matrix4x4Calculation::MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix = Matrix4x4Calculation::Multiply(uvTransformMatrix, Matrix4x4Calculation::MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix;


	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetSpriteRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetSpriteGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, materialResource_->GetGPUVirtualAddress());
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, transformationMatrixResource_->GetGPUVirtualAddress());
	
	
	if (textureHandle_ != 0u) {
		Ellysia::TextureManager::GetInstance()->GraphicsCommand(2u,textureHandle_);

	}
	
	//今度はこっちでドローコールをするよ
	//描画(DrawCall)6個のインデックスを使用し1つのインスタンスを描画。
	directXSetup_->GetCommandList()->DrawIndexedInstanced(6u, 1u, 0u, 0u, 0u);


}

void Ellysia::Sprite::Draw(const uint32_t& texturehandle){
	//非表示にするかどうか
	if (isInvisible_ == true) {
		return;
	}


	//完全に透明になったら通す必要が無いよね
	if (color_.w <= 0.0f) {
		return;
	}


	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//今回は「Sprite」なので三角形が2枚必要
	//-------------
	//|\		  |
	//|  \		  |
	//|    \	  |
	//|      \    |
	//|         \ |
	//-------------

	//13
	//02




	//書き込むためのアドレスを取得
	vertexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&vertexData_));

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;


	float texLeft = 0.0f;
	float texRight = 1.0f;
	float texTop = 0.0f;
	float texBottom = 1.0f;

	//UVをいじりたいとき設定するもの
	if (isUVSetting_ == true) {
		//uv
		texLeft = textureLeftTop_.x / resourceDesc_.Width;
		texRight = (textureLeftTop_.x + textureSize_.x) / resourceDesc_.Width;
		texTop = textureLeftTop_.y / resourceDesc_.Height;
		texBottom = (textureLeftTop_.y + textureSize_.y) / resourceDesc_.Height;
	}




	//左右反転
	if (isFlipX_ == true) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_ == true) {
		top = -top;
		bottom = -bottom;
	}


	//1枚目の三角形
	//左下
	vertexData_[LeftBottom].position = { left,bottom,0.0f,1.0f };
	vertexData_[LeftBottom].texCoord = { texLeft,texBottom };

	//左上
	vertexData_[LeftTop].position = { left,top,0.0f,1.0f };
	vertexData_[LeftTop].texCoord = { texLeft,texTop };

	//右下
	vertexData_[RightBottom].position = { right,bottom,0.0f,1.0f };
	vertexData_[RightBottom].texCoord = { texRight,texBottom };


	//右上
	vertexData_[RightTop].position = { right,top,0.0f,1.0f };
	vertexData_[RightTop].texCoord = { texRight,texTop };


	//IndexResourceにデータを書き込む
	//インデックスデータにデータを書き込む
	indexResource_->Map(0u, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0u;
	indexData_[1] = 1u;
	indexData_[2] = 2u;
	indexData_[3] = 1u;
	indexData_[4] = 3u;
	indexData_[5] = 2u;



	//トランスフォームデータに書き込み
	transformationMatrixResource_->Map(0u, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));

	//座標の再設定
	Vector3 newPosition = {};
	if (isBack_ == true) {
		newPosition = { .x = position_.x,.y = position_.y,.z = 1.0f };
	}
	else {
		newPosition = { .x = position_.x,.y = position_.y,.z = 0.0f };
	}


	//アフィン行列を計算
	Matrix4x4 affineMatrix = Matrix4x4Calculation::MakeAffineMatrix({ scale_.x,scale_.y,1.0f }, { 0.0f,0.0f,rotate_ }, newPosition);
	//遠視投影行列を計算
	Matrix4x4 viewMatrix = Matrix4x4Calculation::MakeIdentity4x4();
	//プロジェクション行列を計算
	Matrix4x4 projectionMatrix = Matrix4x4Calculation::MakeOrthographicMatrix(0.0f, 0.0f, float(Ellysia::WindowsSetup::GetInstance()->GetClientWidth()), float(Ellysia::WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSprite = Matrix4x4Calculation::Multiply(affineMatrix, Matrix4x4Calculation::Multiply(viewMatrix, projectionMatrix));

	transformationMatrixData_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData_->World = Matrix4x4Calculation::MakeIdentity4x4();

	transformationMatrixResource_->Unmap(0u, nullptr);


	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	//ライティングしない
	materialData_->lightingKinds = 0;
	materialData_->shininess = 0.0f;

	Matrix4x4 uvTransformMatrix = Matrix4x4Calculation::MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix = Matrix4x4Calculation::Multiply(uvTransformMatrix, Matrix4x4Calculation::MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix = Matrix4x4Calculation::Multiply(uvTransformMatrix, Matrix4x4Calculation::MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix;


	//コマンドを積む
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(pipelineManager_->GetSpriteRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(pipelineManager_->GetSpriteGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0u, 1u, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0u, materialResource_->GetGPUVirtualAddress());
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1u, transformationMatrixResource_->GetGPUVirtualAddress());


	if (texturehandle != 0u) {
		Ellysia::TextureManager::GetInstance()->GraphicsCommand(2u, texturehandle);

	}

	//今度はこっちでドローコールをするよ
	//描画(DrawCall)6個のインデックスを使用し1つのインスタンスを描画。
	directXSetup_->GetCommandList()->DrawIndexedInstanced(6u, 1u, 0u, 0u, 0u);


}
