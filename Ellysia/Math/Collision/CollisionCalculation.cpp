#include "CollisionCalculation.h"

#include <corecrt_math.h>
#include <numbers>
#include <cmath>
#include <imgui.h>

#include "SingleCalculation.h"
#include <VectorCalculation.h>


bool CollisionCalculation::IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point){
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

bool CollisionCalculation::IsFanCollision(const Fan2D& fan, const Vector2& point){

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
    Vector2 normalizedFanAndPoint = VectorCalculation::Normalize(vectorFanAndPont);
    //向いている方向
    Vector2 direction = VectorCalculation::Normalize(fan.direction);


    //内積を求める
    //左右一致するはず
    //左側
    float centerAndLSDot = SingleCalculation::Dot(direction, fan.leftVector);
    //右側
    float centerAndRSDot = SingleCalculation::Dot(direction, fan.rightVector);
    //ターゲット
    float centerAndTargetDot = SingleCalculation::Dot(direction, normalizedFanAndPoint);

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

bool CollisionCalculation::IsFanCollision(const Fan3D& fan, const Vector3& point){
    
    //扇と点のベクトルを求める
    Vector3 vectorFanAndPont = { 
        .x = point.x - fan.position.x, 
        .y = point.y - fan.position.y, 
        .z = point.z - fan.position.z 
    };
    
    //距離を求める
    float distance = sqrtf(vectorFanAndPont.x * vectorFanAndPont.x + vectorFanAndPont.y * vectorFanAndPont.y+ vectorFanAndPont.z* vectorFanAndPont.z);

    //設定した扇の半径より大きい場合
    //範囲外なのでfalseを返す
    if (fan.length < distance) {
        return false;
    }

    //向きだけが欲しいので正規化をする
    Vector3 normalizedFanAndPoint = VectorCalculation::Normalize(vectorFanAndPont);

    //向いている方向
    Vector3 direction = VectorCalculation::Normalize(fan.direction);

    
    //横方向
    //左
    Vector3 leftOriginDirection = {
        .x = std::cosf(fan.centerRadian + fan.sideThetaAngle),
        .y = std::sinf(fan.centerPhi),
        .z = std::sinf(fan.centerRadian + fan.sideThetaAngle)
    };
    //右
    Vector3 rightOriginDirection = { 
        .x = std::cosf(fan.centerRadian - fan.sideThetaAngle),
        .y = std::sinf(fan.centerPhi),
        .z = std::sinf(fan.centerRadian - fan.sideThetaAngle)
    };
    
   

    //縦方向
    //縦
    float phi = fan.centerPhi;
    const float UP_OFFSET = 10.0f;
    const float DOWN_OFFSET = 30.0f;
    float radian = UP_OFFSET * std::numbers::pi_v<float> / 180.0f;
    float downRadian = DOWN_OFFSET * std::numbers::pi_v<float> / 180.0f;

    
    if (phi > radian ||phi< -downRadian) {
        return false;
    }

    Vector3 upOriginDirection = { 
        .x = 0.0f, 
        .y = std::sinf(phi + radian),
        .z = std::cosf(phi + radian),
    };
    //下
    Vector3 downOriginDirection = {
        .x = 0.0f,
        .y = std::sinf(phi - radian),
        .z = std::cosf(phi - radian),
    };

#ifdef _DEBUG
    ImGui::Begin("FanDirectionOrigin");
    ImGui::InputFloat3("FanAndPoint", &vectorFanAndPont.x);
    ImGui::InputFloat3("Up", &upOriginDirection.x);
    ImGui::InputFloat3("Down", &downOriginDirection.x);
    ImGui::InputFloat3("Left", &leftOriginDirection.x);
    ImGui::InputFloat3("Right", &rightOriginDirection.x);
    ImGui::InputFloat("Phi", &phi);
    ImGui::InputFloat("radian", &radian);


    ImGui::End();

#endif


    //それぞれを正規化する
    Vector3 rightDirection = VectorCalculation::Normalize(rightOriginDirection);
    Vector3 leftDirection = VectorCalculation::Normalize(leftOriginDirection);
    Vector3 upDirection = VectorCalculation::Normalize(upOriginDirection);
    Vector3 downDirection = VectorCalculation::Normalize(downOriginDirection);

    




    //縦と横で2次元にして別々で計算した方が良いかも
    //上手く出来なかったから
    Vector2 newLeftDirection = { .x = leftDirection.x,.y = leftDirection.z };
    Vector2 newRightDirection = { .x = rightDirection.x,.y = rightDirection.z };
    Vector2 newUpDirection = { .x = upDirection.z,.y = upDirection.y };
    Vector2 newDownDirection = { .x = downDirection.z,.y = downDirection.y };
    Vector2 newXZDirection = { .x = normalizedFanAndPoint.x,.y = normalizedFanAndPoint.z };
    Vector2 newZYDirection = { .x = normalizedFanAndPoint.z,.y = normalizedFanAndPoint.y };

#ifdef _DEBUG
    ImGui::Begin("FanDirection");
    ImGui::InputFloat3("FanAndPoint", &normalizedFanAndPoint.x);
    ImGui::InputFloat3("Up", &upDirection.x);
    ImGui::InputFloat3("Down", &downDirection.x);
    ImGui::InputFloat3("Left", &leftDirection.x);
    ImGui::InputFloat3("Right", &rightDirection.x);
    ImGui::End();


#endif
    //左側
    float dotLS = SingleCalculation::Dot({.x = direction.x,.y = direction.z }, newLeftDirection);
    //右側
    float dotRS = SingleCalculation::Dot({.x = direction.x,.y = direction.z }, newRightDirection);
    //ターゲット
    float dotCenterXZ = SingleCalculation::Dot({ .x = direction.x,.y = direction.z }, newXZDirection);
    //これより小さくなったら当たっていないと返す
    
    
    //上側
    float dotUS = SingleCalculation::Dot({ .x = direction.z,.y = direction.y }, newUpDirection);
    //下側
    float dotDS = SingleCalculation::Dot({ .x = direction.z,.y = direction.y }, newDownDirection);
    //ターゲット
    float dotCenterYZ = SingleCalculation::Dot({.x = direction.z,.y = direction.y }, newZYDirection);

#ifdef _DEBUG



    ImGui::Begin("FanDot");
    ImGui::InputFloat("LSDot", &dotLS);
    ImGui::InputFloat("RSDot", &dotRS);
    ImGui::InputFloat("UpDot", &dotUS);
    ImGui::InputFloat("DownDot", &dotDS);

    ImGui::InputFloat("dotCenterXZ", &dotCenterXZ);
    ImGui::InputFloat("dotCenterYZ", &dotCenterYZ);

    ImGui::End();



    //角度を測って当たり判定をとろうと思ったけど一周した後が大変なことになっている
    //内積でやった方が良いことに気づいたのでそちらで計算する


#endif // _DEBUG

    if (dotCenterXZ < dotLS ||
        dotCenterXZ < dotRS ) {
        return false;
    }
    

    return true;

}


bool CollisionCalculation::IsCollisionPlaneAndPoint(const Vector3& position, const Plane& plane){
    //平面上にいない時除外する
    if (position.y < plane.position.y) {
        return false;
    }

   //範囲外の時除外する
    if(position.x < plane.position.x - plane.width / 2.0f ||
        position.x > plane.position.x + plane.width / 2.0f ||
        position.z < plane.position.z - plane.length / 2.0f ||
        position.z > plane.position.z + plane.length / 2.0f){
        
        return false;
    }


    return true;
}
