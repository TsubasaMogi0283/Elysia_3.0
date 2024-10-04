#include "LevelDataObjectCollision.h"



void LevelDataObjectCollision::Initialze(std::string& type, Vector3& position, Vector3& size){
	if (type == "Sphere") {
		collisionType_ = CollisionType::SphereType;
	}
	else if (type == "BOX"||type=="AABB") {
		collisionType_ = CollisionType::AABBType;
	}



}

void LevelDataObjectCollision::Update(Vector3& position)
{
}


