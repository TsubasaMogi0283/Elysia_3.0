#include "Key.h"
#include "ModelManager.h"
#include "Easing.h"
#include "VectorCalculation.h"
#include "TextureManager.h"

void Key::Initialize(const uint32_t& modelhandle,const Vector3& position){
	//モデルの生成
	model_.reset(Ellysia::Model::Create(modelhandle));

	//スケールのサイズ
	const float SCALE = 0.4f;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//拡縮
	worldTransform_.scale = {.x = SCALE,.y = SCALE ,.z = SCALE };
	//座標
	worldTransform_.translate = position;
	originalPositionY_ = position.y;
	//半径
	//と言っても少しだけ大きくして取りやすくする
	radius_ = SCALE * 4.0f;
	//取得されたかどうかのフラグ
	isPickUp_ = false;

	//マテリアルの初期化
	material_.Initialize();
	//ライティングの種類
	material_.lightingKinds_ = SpotLighting;
	//色
	material_.color_ = {.x = 1.0f,.y = 1.0f,. z = 0.0f,. w = 1.0f };


	//鍵のスプライト
	uint32_t textureHandle = Ellysia::TextureManager::GetInstance()->LoadTexture("Resources/Item/Key/Key.png");
	sprite_.reset(Ellysia::Sprite::Create(textureHandle, {.x=0.0f,.y=0.0f}));
	//アンカーポイントを設定する
	const Vector2 ANCHOR_POINT = { .x = 0.5f,.y = 0.5f };
	sprite_->SetAnchorPoint(ANCHOR_POINT);
	//最初は非表示
	sprite_->SetInvisible(true);



#ifdef _DEBUG
	uint32_t debugModelHandle = Ellysia::ModelManager::GetInstance()->LoadModelFile("Resources/Model/Sample/Cube", "cube.obj");
	//モデルの生成
	debugModel_.reset(Ellysia::Model::Create(debugModelHandle));
	//ワールドトランスフォームの初期化
	debugWorldTransform_.Initialize();
	//マテリアルの初期化
	debugMaterial_.Initialize();
	debugMaterial_.lightingKinds_ = SpotLighting;
#endif // _DEBUG


}

void Key::Update(){
	
	if (isPickUp_ == false) {
		//回転の大きさ
		const float ROTATE_AMOUNT = 0.1f;
		//角度の計算
		heightTheta_ += ROTATE_AMOUNT;
		//上下する
		worldTransform_.translate.y = std::sinf(heightTheta_) * MOVE_AMOUNT_ + originalPositionY_;
		//回転
		worldTransform_.rotate.y += ROTATE_AMOUNT;

	}
	else {
		//上昇回転
		RiseAndRotate();
		//スプライトが動く
		SpriteMove();
	}

	//デバッグ用
	debugWorldTransform_.translate = GetWorldPosition();
	debugWorldTransform_.scale = { .x = 0.1f,.y = 0.1f,.z = 0.1f };
	debugMaterial_.color_.w = 0.3f;

	//ワールドトランスフォームの更新
	worldTransform_.Update();
	debugWorldTransform_.Update();
	//マテリアルの更新
	material_.Update();
	debugMaterial_.Update();

}

void Key::DrawModel(const Camera& camera,const SpotLight& spotLight){
	//鍵(モデル)の描画
	model_->Draw(worldTransform_, camera, material_, spotLight);

#ifdef _DEBUG
	//デバッグ用の描画
	debugModel_->Draw(debugWorldTransform_, camera, debugMaterial_);
#endif // _DEBUG


}

void Key::DrawSprite(){
	//鍵(スプライト)の描画
	sprite_->Draw();
}

void Key::RiseAndRotate(){

	//上昇する前の座標を記録
	if (isWritePreUpPosition_==false) {
		preUpPosition_ = GetWorldPosition();
		isWritePreUpPosition_ = true;
	}

	//イージングを使い急上昇する感じを出す
	upT_ += 0.001f;
	float t = Easing::EaseOutSine(upT_);
	//高さ
	const float HEIGHT =0.5f;
	//線形補間を使い
	float lerpedPositionY = SingleCalculation::Lerp(preUpPosition_.y, preUpPosition_.y+HEIGHT, t);
	worldTransform_.translate.y = lerpedPositionY;

	//高速回転
	const float RAPID_ROTATE_AMOUNT = 0.5f;
	worldTransform_.rotate.y += RAPID_ROTATE_AMOUNT;

	//4回転した後に拡縮
	if (worldTransform_.rotate.y > std::numbers::pi_v<float>*12.0f) {
		const float SCALE_DOWM_VALUE = -0.05f;
		const Vector3 SCALE_DOWN = { .x = SCALE_DOWM_VALUE,.y = SCALE_DOWM_VALUE ,.z = SCALE_DOWM_VALUE };
		worldTransform_.scale = VectorCalculation::Add(worldTransform_.scale, SCALE_DOWN);
	}

	//消える
	if (worldTransform_.scale.x < 0.0f&&
		worldTransform_.scale.y < 0.0f&&
		worldTransform_.scale.z < 0.0f) {
		worldTransform_.scale.x = 0.0f;
		worldTransform_.scale.y = 0.0f;
		worldTransform_.scale.z = 0.0f;
		//スプライトが動く
		isSpriteMove_ = true;
		
	}



}

void Key::SpriteMove(){
	if (isSpriteMove_ == true) {
		//表示
		sprite_->SetInvisible(false);

		scaleT_ += 0.01f;
		//サイズの設定
		float scaleSize = std::sinf(scaleT_ *std::numbers::pi_v<float>);
		sprite_->SetScale({ .x = scaleSize ,.y = scaleSize });

		//回転の設定
		spriteRotate_ += 0.2f;
		sprite_->SetRotate(spriteRotate_);


		//始点
		const Vector2 SPRITE_STRAT_POSITION_ = { .x = 680,.y = 600.0f };
		//終点
		const Vector2 SPRITE_END_POSITION_ = { .x = 64.0f * 2.0f,.y = 20.0f };


		//座標の設定
		Vector2 position = VectorCalculation::Lerp(SPRITE_STRAT_POSITION_, SPRITE_END_POSITION_, scaleT_);
		sprite_->SetPosition(position);

		//消える
		if (scaleT_ >= 1.0f) {
			isDelete_ = true;
			isSpriteMove_ = false;
		}



	}
}

void Key::DisplayImGui(){
#ifdef _DEBUG
	ImGui::Begin("鍵のスプライト");
	ImGui::InputFloat("scaleT_", &scaleT_);
	ImGui::InputFloat3("回転", &worldTransform_.rotate.y);
	ImGui::End();
#endif // _DEBUG

}


