#include "Collision.h"
#include <VectorCalculation.h>
#include <corecrt_math.h>



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

bool IsFanCollision(Fan& fan, Vector3& point){

    //扇と点のベクトルを求める
    Vector3 vector = Subtract(point, fan.position);
    float distance = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    if (fanLength < distance) {
        return true;
    }


    return false;
}
