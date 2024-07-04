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



    //今は2次元でやっているが後で3次元に拡張する

    //扇と点のベクトルを求める
    Vector2 vectorFanAndPont = { point.x - fan.position.x, point.y - fan.position.y };
    //距離を求める
    float distance = sqrtf(vectorFanAndPont.x * vectorFanAndPont.x + vectorFanAndPont.y * vectorFanAndPont.y);


    //設定した扇の半径より大きい場合
    //範囲外なのでfalseを返す
    if (fan.length < distance) {
        return false;
    }

    
   //向きだけが欲しいので正規化をする
    Vector2 normalizedFanAndPoint = Normalize(vectorFanAndPont);

    //向いている方向
    Vector2 direction = Normalize(fan.direction);

    //内積を求める
    //左右一致するはず
    //左側
    float centerAndLSDot = Dot(direction, fan.leftVector);
    //右側
    float centerAndRSDot = Dot(direction, fan.rightVector);
    //ターゲット
    float centerAndTargetDot = Dot(direction, normalizedFanAndPoint);

#ifdef _DEBUG

    ImGui::Begin("FanDirection");
    ImGui::InputFloat2("MainDirection", &direction.x);
    ImGui::InputFloat2("Vector", &vectorFanAndPont.x);
    ImGui::InputFloat2("NormalizedVector", &normalizedFanAndPoint.x);
    ImGui::End();

    ImGui::Begin("FanDot");
    ImGui::InputFloat("LSDot", &centerAndLSDot);
    ImGui::InputFloat("RSDot", &centerAndRSDot);
    ImGui::InputFloat("CTDot", &centerAndTargetDot);

    
    ImGui::End();



    //角度を測って当たり判定をとろうと思ったけど一周した後が大変なことになっている
    //内積でやった方が良いことに気づいたのでそちらで計算する
    
    //ライト(プレイヤー)とターゲットとの角度
    float targetRadian = std::atan2f(normalizedFanAndPoint.y, normalizedFanAndPoint.x);

    //持ってきた値を代入
    float leftSideRadian = fan.leftSideRadian;
    float newLeftSideRadian = fan.centerRadian + fan.sideAngle;
    float rightSideRadian = fan.rightSideRadian;


    //度数法
    ImGui::Begin("FanRadian");
    ImGui::InputFloat("LSRadian", &leftSideRadian);
    ImGui::InputFloat("NewLSRadian", &newLeftSideRadian);
    ImGui::InputFloat("TargetRadian", &targetRadian);
    ImGui::InputFloat("RSRadian", &rightSideRadian);

    ImGui::End();

    //弧度法だとデバッグ時分かりずらと思ったので度数法に直す
    //変換する関数を作った方が良いかも
    float leftSideDigree = fan.leftSideRadian * (180.0f / std::numbers::pi_v<float>);
    float rightSideDigree = fan.rightSideRadian * (180.0f / std::numbers::pi_v<float>);
    float targetDigree = targetRadian * (180.0f / std::numbers::pi_v<float>);
    //扇中心の角度
    float centerDigree = fan.centerRadian * (180.0f / std::numbers::pi_v<float>);

    ImGui::Begin("FanDigree");
    ImGui::InputFloat("LSTheta", &leftSideDigree);
    ImGui::InputFloat("CenterThetaOrigin", &centerDigree);
    ImGui::InputFloat("NewTargetTheta", &targetDigree);
    ImGui::InputFloat("RSTheta", &rightSideDigree);
    ImGui::End();

    
#endif // _DEBUG

    //最小でcenterAndLSDotまたはcenterAndRSDot
    //これより小さくなったら当たっていないと返す
    if (centerAndTargetDot < centerAndLSDot ||
        centerAndTargetDot < centerAndRSDot) {
        return false;
    }



    return true;
}
