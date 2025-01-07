#include "TitleScene.h"
#include <imgui.h>
#include <numbers>


#include "Input.h"
#include "GameScene/GameScene.h"
#include "GameManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"

#include "TextureManager.h"
#include "LevelDataManager.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>

TitleScene::TitleScene(){
	//テクスチャ管理クラスの取得
	textureManager_ = TextureManager::GetInstance();
	//入力クラスの取得
	input_ = Input::GetInstance();
	//レベルエディタ管理クラスの取得
	levelDataManager_ = LevelDataManager::GetInstance();
}

void TitleScene::Initialize(){

	//ロゴ
	uint32_t logoTextureHandle = textureManager_->LoadTexture("Resources/Title/StartText.png");
	//タイトルテクスチャ
	uint32_t titleTextureHandle = textureManager_->LoadTexture("Resources/Title/Title.png");

	//初期化
	const Vector2 INITIAL_POSITION = {.x=0.0f,.y=0.0f};
	//生成
	//テキスト
	text_.reset(Sprite::Create(logoTextureHandle, INITIAL_POSITION));
	//毎系
	backGround_.reset(Sprite::Create(titleTextureHandle, INITIAL_POSITION));


	levelHandle_ = levelDataManager_->Load("TitleStage/TitleStage.json");

	isStart_ = false;
	isFlash_ = true;
	isFastFlash_ = false;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Directional;

	//スポットライトの初期化
	spotLight.Initialize();

	directionalLight_.Initialize();

	//カメラの初期化
	camera_.Initialize();
	//座標
	camera_.translate_ = {.x = 0.0f,.y = 0.0f,.z = -9.8f };


	points_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,2.0f,2.0f},
		{0.0f,4.0f,4.0f},
		{0.0f,2.0f,6.0f},
		{0.0f,0.0f,8.0f},


	};

	//ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();
}



Vector3 TitleScene::CatmullRomPositionLoop(const std::vector<Vector3>& points, float& t) {
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	//区間数は制御点の数-1
	//初期化処理の所のcontrolPointに入っている数を参照してあげる
	size_t division = points.size() - 1;
	//1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	//区間内の始点を0.0f、終点を1.0としたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	//下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = SingleCalculation::Clamp(t_2, 0.0f, 1.0f);

	int index = static_cast<int>(t / areaWidth);
	//区間番号が上限を超えないための計算
	//index = max(index, 4);


	int index0 = index - 1;
	int index1 = index;
	int index2 = index + 1;
	int index3 = index + 2;



	//始点&終点だった場合制御点を設定し直すよ
	//最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}


	//最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;

		//また最初に戻る
		if (t > 1.0f) {
			t = 0.0f;
			index = 0;
			index0 = index;
			index1 = index;
			index2 = index + 1;
			index3 = index + 2;
		}
	}



	//4点の座標
	p0 = points[index0];
	p1 = points[index1];
	p2 = points[index2];
	p3 = points[index3];

#ifdef _DEBUG
	ImGui::Begin("Index");
	ImGui::InputFloat("t2", &t_2);
	ImGui::InputInt("Index", &index);
	ImGui::InputInt("Index0", &index0);
	ImGui::InputInt("Index1", &index1);
	ImGui::InputInt("Index2", &index2);
	ImGui::InputInt("Index3", &index3);
	ImGui::End();
#endif // _DEBUG

	

	//結果
	Vector3 result = VectorCalculation::CatmullRom(points[index0], points[index1], points[index2], points[index3], t_2);
	return result;
}



void TitleScene::Update(GameManager* gameManager){
	//増える時間の値
	const uint32_t INCREASE_VALUE = 1u;
	//Bトリガーの反応する時間
	const uint32_t REACT_TIME = 1u;
	//Bトリガーの反応しない時間
	const uint32_t NO_REACT_TIME = 0u;

	//再スタート時間
	const uint32_t RESTART_TIME = 0u;

	


	//まだボタンを押していない時
	//通常点滅
	if (isFlash_ == true) {
		
		//時間の加算
		flashTime_ += INCREASE_VALUE;

		if (flashTime_ > FLASH_TIME_LIMIT_ * 0 &&
			flashTime_ <= FLASH_TIME_LIMIT_ ) {
			text_->SetInvisible(false);
		}
		if (flashTime_ > FLASH_TIME_LIMIT_ &&
			flashTime_ <= FLASH_TIME_LIMIT_*2) {
			text_->SetInvisible(true);

		}
		if (flashTime_ > FLASH_TIME_LIMIT_*2) {
			flashTime_ = RESTART_TIME;
		}

	}
	

	//コントローラーのBを押すと高速点滅
	if (input_->IsConnetGamePad() == true) {

		//Bボタンを押したとき
		if (input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			bTriggerTime_ += INCREASE_VALUE;

		}
		//押していない
		if ((input_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			bTriggerTime_ = NO_REACT_TIME;
		}

		//反応
		if (bTriggerTime_ == REACT_TIME) {

			isFastFlash_ = true;
		}

	}

	//スペースを押したら高速点滅
	if (input_->IsPushKey(DIK_SPACE) == true) {
		//高速点滅
		isFastFlash_ = true;
	}

	//カウントが増える時間
	const uint32_t INCREASE_COUNT_TIME = 0u;
	//点滅の間隔
	const uint32_t FLASH_INTERVAL = 2u;

	//高速点滅
	if (isFastFlash_ == true) {
		fastFlashTime_ += INCREASE_VALUE;
		if (fastFlashTime_ % FAST_FLASH_TIME_INTERVAL_ == INCREASE_COUNT_TIME) {
			//もう一度学び直したが
			//単純に+1にしたいなら前置インクリメント「++(名前)」がいいらしい

			//加算される前の値を入れたいなら後置インクリメント「(名前)++」にしよう
			++textDisplayCount_;
		}
		//表示
		const uint32_t DISPLAY_MOMENT = 0u;
		if (textDisplayCount_ % FLASH_INTERVAL == DISPLAY_MOMENT) {
			text_->SetInvisible(true);
		}
		//非表示
		else {
			text_->SetInvisible(false);
		}


		//指定した時間を超えたらシーンチェンジ
		if (fastFlashTime_> FAST_FLASH_TIME_LIMIT_) {
			isStart_ = true;
		}
	}
	//脱出
	if (isStart_ == true) {
		gameManager->ChangeScene(new GameScene());
		return;
	}


	//ステージデータの更新
	Listener listener = {
			.position = {},
			.move = {},
	};
	levelDataManager_->SetListener(levelHandle_, listener);

	levelDataManager_->Update(levelHandle_);




	//回転角を求めてワールドトランスフォームの角度に入れる
	cameraT_ += 0.001f;
	//Vector3 tangent = VectorCalculation::CatmullRom(p0, p1, p2, p3, cameraT_);
	//
	////atan(高さ,底辺)
	//camera_.rotate_.y = std::atan2(tangent.x, tangent.z);
	////三角比
	//float velocityXZ = sqrtf((tangent.x * tangent.x) + (tangent.z * tangent.z));
	//camera_.rotate_.x = std::atan2(-tangent.y, velocityXZ);



	
	
	camera_.translate_ = CatmullRomPositionLoop(points_, cameraT_);


	//マテリアルの更新
	material_.Update();
	//スポットライトの更新
	spotLight.Update();
	directionalLight_.Update();
	//カメラの更新
	camera_.Update();

	


#ifdef _DEBUG
	ImGui::Begin("CameraInfo");
	ImGui::InputFloat("T", &cameraT_);
	ImGui::InputFloat3("Translate", &camera_.translate_.x);
	ImGui::InputFloat3("Rotate", &camera_.rotate_.x);
	ImGui::End();


	ImGui::Begin("P");
	
	ImGui::InputFloat3("P0", &p0.x);
	ImGui::InputFloat3("P1", &p1.x);
	ImGui::InputFloat3("P2", &p2.x);
	ImGui::InputFloat3("P3", &p3.x);
	ImGui::End();
#endif // _DEBUG
}

void TitleScene::DrawObject3D(){

	levelDataManager_->Draw(levelHandle_,camera_, material_, directionalLight_);


}

void TitleScene::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void TitleScene::DrawPostEffect(){
	back_->Draw();
}

void TitleScene::DrawSprite(){
	//背景
	//backGround_->Draw();

	//テキスト
	text_->Draw();
	
}
