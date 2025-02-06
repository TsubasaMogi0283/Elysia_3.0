#include "TitleRailCamera.h"

#include <cmath>
#include <algorithm>

#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "Matrix4x4Calculation.h"
#include "Quaternion.h"
#include "Calculation/QuaternionCalculation.h"

void TitleRailCamera::Initialize(){

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//カメラの初期化
	camera_.Initialize();

	//LevelEditorに入れたいね
	//一旦CSVにしてみる
	//制御点
	points_ = {
		{0.0f,3.0f,-75.0f},
		{10.0f,3.0f,-75.0f},
		{20.0f,3.0f,-75.0f},
		{30.0f,3.0f,-75.0f},
		{40.0f,3.0f,-75.0f},
		{50.0f,3.0f,-75.0f},
		{60.0f,3.0f,-70.0f},
		{70.0f,3.0f,-65.0f},
		{75.0f,3.0f,-60.0f},
		{75.0f,3.0f,-45.0f},
		{75.0f,3.0f,-30.0f},
		{75.0f,3.0f,-20.0f},
		{75.0f,3.0f,-10.0f},
		{75.0f,3.0f,-5.0f},
		{75.0f,3.0f,0.0f},
		{75.0f,3.0f,5.0f},
		{75.0f,3.0f,10.0f},
		{75.0f,3.0f,20.0f},
		{75.0f,3.0f,25.0f},
		{75.0f,3.0f,30.0f},
		{75.0f,3.0f,40.0f},
		{75.0f,3.0f,50.0f},
		{70.0f,3.0f,60.0f},
		{65.0f,3.0f,70.0f},
		{60.0f,3.0f,75.0f},
		{50.0f,3.0f,75.0f},
		{40.0f,3.0f,75.0f},
		{30.0f,3.0f,75.0f},
		{20.0f,3.0f,75.0f},
		{10.0f,3.0f,75.0f},
		{0.0f,3.0f,75.0f},
		{-10.0f,3.0f,75.0f},
		{-20.0f,3.0f,75.0f},
		{-30.0f,3.0f,75.0f},
		{-40.0f,3.0f,75.0f},
		{-50.0f,3.0f,75.0f},
		{-60.0f,3.0f,75.0f},
		{-70.0f,3.0f,70.0f},
		{-75.0f,3.0f,65.0f},



		{-75.0f,3.0f,60.0f},
		{-75.0f,3.0f,50.0f},
		{-75.0f,3.0f,40.0f},
		{-75.0f,3.0f,30.0f},
		{-75.0f,3.0f,20.0f},
		{-75.0f,3.0f,10.0f},
		{-75.0f,3.0f,0.0f},
		{-75.0f,3.0f,-10.0f},
		{-75.0f,3.0f,-20.0f},
		{-75.0f,3.0f,-30.0f},
		{-75.0f,3.0f,-40.0f},
		{-75.0f,3.0f,-50.0f},
		{-70.0f,3.0f,-60.0f},
		{-60.0f,3.0f,-70.0f},


		{-50.0f,3.0f,-75.0f},
		{-40.0f,3.0f,-75.0f},
		{-30.0f,3.0f,-75.0f},
		{-20.0f,3.0f,-75.0f},
		{-10.0f,3.0f,-75.0f},
		{0.0f,3.0f,-75.0f},
	};

}

void TitleRailCamera::Update(){

	//線形補間
	cameraT_ += 0.0001f;
	
	//少し先のTの値を見て次の位置の計算をする
	float nextT = cameraT_ + 0.001f;
	//ループ時の補正
	if (nextT > 1.0f) {
		nextT -= 1.0f;
	};

	//座標
	worldTransform_.translate = CatmullRomPositionLoop(points_, cameraT_);

	//現在の位置
	Vector3 currentPosition = worldTransform_.translate;
	//次の座標
	Vector3 nextPosition = CatmullRomPositionLoop(points_, nextT);
	//差分
	Vector3 difference = VectorCalculation::Subtract(nextPosition, currentPosition);
	//進行方向ベクトル
	direction_ = VectorCalculation::Normalize(difference);

	//Y軸の回転
	worldTransform_.rotate.y = std::atan2(direction_.x, direction_.z);
	//X軸の回転
	//XZの長さ.まず先に長さを求めてから回転を求める.
	float velocityXZ = std::sqrt((direction_.x * direction_.x) + (direction_.z * direction_.z));
	worldTransform_.rotate.x = std::atan2(-direction_.y, velocityXZ);

	//ワールド行列を計算
	worldTransform_.worldMatrix = Matrix4x4Calculation::MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	//ビュー行列の計算
	camera_.viewMatrix = Matrix4x4Calculation::Inverse(worldTransform_.worldMatrix);



#ifdef _DEBUG
	//IMGui表示用
	DisplayImGui();
#endif // _DEBUG

	
}

void TitleRailCamera::DisplayImGui(){
	ImGui::Begin("レールカメラ");
	ImGui::DragFloat3("座標", &worldTransform_.translate.x);
	ImGui::InputFloat3("方向", &direction_.x);
	ImGui::SliderFloat("T", &cameraT_, 0.0f, 1.0f);
	ImGui::InputFloat3("回転", &worldTransform_.rotate.x);
	ImGui::End();

}

Vector3 TitleRailCamera::CatmullRomPositionLoop(const std::vector<Vector3>& points, const float& t) {
	//4点以上で動作するので
	//それ未満は止める
	assert(points.size() >= 4);

	//区間数は制御点の数-1
	//初期化処理の所の制御点に入っている数を参照してあげる
	size_t division = points.size() - 1;
	//1区間の長さ(全体を1.0とした割合)
	float areaLength = 1.0f / division;

	//区間内の始点を0.0f、終点を1.0としたときの現在位置
	float t_2 = std::fmod(t, areaLength) * division;
	//下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	int index = static_cast<int>(t / areaLength);
	int index0 = index - 1;
	int index1 = index;
	int index2 = index + 1;
	int index3 = index + 2;



	//始点&終点だった場合制御点を設定し直すよ
	//最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}

	t;
	//最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;

		//また最初に戻る
		if (cameraT_ > 1.0f) {
			cameraT_ = 0.0f;
			index = 0;
			index0 = index;
			index1 = index;
			index2 = index + 1;
			index3 = index + 2;
		}
	}

	//4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];


	//結果
	Vector3 result = VectorCalculation::CatmullRom(p0, p1, p2, p3, t_2);
	return  result;
}
