#include "Collision.h"
#include <VectorCalculation.h>
#include <corecrt_math.h>
#include <numbers>
#include <cmath>
#include <imgui.h>


//AABBとPointの当たり判定
bool IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point){
#pragma region 手前
    //左上
    Vector3 frontLeftTop = { aabb.min.x,aabb .max.y,aabb .min.z};
    //右上
    Vector3 frontRightTop = { aabb.max.x,aabb.max.y,aabb.min.z };
    //右下
    Vector3 frontRightBottom= { aabb.max.x,aabb.min.y,aabb.min.z };
    //左下
    Vector3 frontLeftBottom = { aabb.min.x,aabb.min.y,aabb.min.z };
    
#pragma endregion

#pragma region 奥
    //左上
    Vector3 backLeftTop = { aabb.min.x,aabb.max.y,aabb.max.z };
    //右上
    Vector3 backRightTop = { aabb.max.x,aabb.max.y,aabb.max.z };
    //右下
    Vector3 backRightBottom = { aabb.max.x,aabb.min.y,aabb.max.z };
    //左下
    Vector3 backLeftBottom = { aabb.min.x,aabb.min.y,aabb.max.z };

#pragma endregion

    //横
    //縦
    //奥行
    if ((point.x >= aabb.min.x && point.x <= aabb.max.x) &&
        (point.y >= aabb.min.y && point.y <= aabb.max.y) &&
        (point.z >= aabb.min.z && point.z <= aabb.max.z)) {
        return true;
    }
    else {
        return false;
    }


}

bool IsFanAndPointCollision(const Fan& fan, const Vector2& point){

    //参考
    //https://yttm-work.jp/collision/collision_0008.html

    //扇と点のベクトルを求める
//    Vector2 vector = { point.x-fan.position.x, point.y-fan.position.y };
//    float distance = sqrtf(vector.x * vector.x + vector.y * vector.y );
//
//#ifdef _DEBUG
//    ImGui::Begin("FanCollision");
//    ImGui::InputFloat2("Vector", &vector.x);
//    ImGui::InputFloat("Distance", &distance);
//    ImGui::End();
//#endif // _DEBUG
//
//    //範囲外の場合早期リターン
//    //この時は扇の半径の長さより長い時である
//    if (fan.length < distance) {
//        return false;
//    }
//    //扇を2等分する線のベクトルを求める
//    float directionRad = fan.directionRadian;
//    Vector2 fanDirection = { std::cosf(directionRad),std::sinf(directionRad)};
//    //Vector2 fanDirection = fan.devidDirection;
//
//    //扇と点のベクトルを単位ベクトルにする
//    Vector2 normalFanToPoint = {};
//    normalFanToPoint.x = vector.x / distance;
//    normalFanToPoint.y = vector.y / distance;
//
//    //内積計算
//    float dot = normalFanToPoint.x * fanDirection.y + normalFanToPoint.y * fanDirection.y;
//
//    //2で割る
//    float fanCos = std::cosf(directionRad / 2.0f);
//
//#ifdef _DEBUG
//    ImGui::Begin("FanCollision2");
//    ImGui::InputFloat("DirectionRad", &directionRad);
//    ImGui::InputFloat("NormalFanToPoint", &normalFanToPoint.x);
//    ImGui::InputFloat("Dot", &dot);
//    ImGui::InputFloat("FanCos", &fanCos);
//    ImGui::End();
//#endif // _DEBUG
//
//    //点が扇の範囲内にあるかを比較する
//    if (fanCos > dot) {
//        //当たっていない
//        return false;
//    }



    //今は2次元でやっているが後で3次元に拡張する


    //扇と点のベクトルを求める
    Vector2 vector = { point.x - fan.position.x, point.y - fan.position.y };
    float distance = sqrtf(vector.x * vector.x + vector.y * vector.y);

#ifdef _DEBUG
    ImGui::Begin("FanCollision1");
    ImGui::InputFloat2("Vector", &vector.x);
    ImGui::InputFloat("Distance", &distance);
    ImGui::End();
#endif // _DEBUG

    //扇の半径より大きい場合
    //範囲外なのでfalseを返す
    if (fan.length < distance) {
        return false;
    }



    //atan2で扇の中心から目的地までの角度を求めたい
    //向きだけが欲しいので正規化をする
    Vector2 normalizedFanPointVector = Normalize(vector);

    Vector2 direction = fan.divideDirection;

    //
    float targetTheta = atan2f(normalizedFanPointVector.x, normalizedFanPointVector.y);
    //ずれが発生しているのでその分足す
    float newTargetTheta = targetTheta + std::numbers::pi_v<float> / 2.0f;

    float leftTheta = fan.leftSideRadian;
    float centerTheta = fan.centerRadian;
    float rightTheta = fan.rightSideRadian;
    //点の角度を求める
#ifdef _DEBUG
    ImGui::Begin("FanCollision2");
    ImGui::InputFloat("TargetTheta", &targetTheta);
    ImGui::InputFloat("LeftTheta", &leftTheta);
    ImGui::InputFloat("CenterTheta", &centerTheta);
    ImGui::InputFloat("RightTheta", &rightTheta);
    ImGui::InputFloat("NewTargetTheta", &newTargetTheta);
    ImGui::End();
#endif // _DEBUG



    //「θL>=θT>=θR」この時だけ扇内にいる
    //左端より大きい時と右端より小さい角度の時の場合
    if (fan.leftSideRadian < newTargetTheta ||
        newTargetTheta<fan.rightSideRadian) {
        return false;
    }





    

    return true;
}
