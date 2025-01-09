#include "TitleRailCamera.h"



#include <cmath>

#include "VectorCalculation.h"
#include "SingleCalculation.h"
#include "Matrix4x4Calculation.h"
#include "Quaternion.h"
#include "Calculation/QuaternionCalculation.h"

void TitleRailCamera::Initialize(){

	worldTransform_.Initialize();
	camera_.Initialize();

	//制御点
	points_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,2.0f,4.0f},
		{0.0f,4.0f,8.0f},
		{0.0f,2.0f,12.0f},
		{0.0f,0.0f,16.0f},
	};

}


Vector3 TitleRailCamera::CatmullRomPositionLoop(const std::vector<Vector3>& points,  float t) {
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	//区間数は制御点の数-1
	//初期化処理の所の制御点に入っている数を参照してあげる
	size_t division = points.size() - 1;
	//1区間の長さ(全体を1.0とした割合)
	float areaLength = 1.0f / division;

	//区間内の始点を0.0f、終点を1.0としたときの現在位置
	t_2 = std::fmod(t, areaLength) * division;
	//下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = SingleCalculation::Clamp(0.0f, 1.0f, t_2);

	int index = static_cast<int>(t / areaLength);
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
	return  VectorCalculation::CatmullRom(p0, p1, p2, p3, t_2);
}


void TitleRailCamera::Update(){



	//回転角を求めてワールドトランスフォームの角度に入れる
	cameraT_ += 0.001f;
	
	//targetとeyeの差分ベクトル(forward)から02_09_ex1より
	//回転角を求めてワールドトランスフォームの角度に入れる
	Vector3 tangent = VectorCalculation::CatmullRom(p0, p1, p2, p3, t_2);



	if (cameraT_ >= 0.5f) {
		tangent;
	}
	//atan(高さ,底辺)
	//ここは09aとだいたい同じ
	worldTransform_.rotate.y = std::atan2(tangent.x, tangent.z);
	//三角比
	float velocityXZ = sqrtf((tangent.x * tangent.x) + (tangent.z * tangent.z));
	worldTransform_.rotate.x = std::atan2(-tangent.y, velocityXZ);
	


	worldTransform_.translate = CatmullRomPositionLoop(points_, cameraT_);

	worldTransform_.worldMatrix = Matrix4x4Calculation::MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	
	//ビュー行列の計算
	camera_.viewMatrix = Matrix4x4Calculation::Inverse(worldTransform_.worldMatrix);



	






#ifdef _DEBUG
	ImGui::Begin("CameraInfo");
	ImGui::DragFloat3("Position", &worldTransform_.translate.x);
	ImGui::SliderFloat("T", &cameraT_,0.0f,1.0f);
	ImGui::InputFloat3("Rotate", &worldTransform_.rotate.x);
	ImGui::End();


	ImGui::Begin("P");

	ImGui::InputFloat3("P0", &p0.x);
	ImGui::InputFloat3("P1", &p1.x);
	ImGui::InputFloat3("P2", &p2.x);
	ImGui::InputFloat3("P3", &p3.x);
	ImGui::End();
#endif // _DEBUG

}
