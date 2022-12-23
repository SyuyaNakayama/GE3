#pragma once
#include "Collider.h"
#include <vector>

class CollisionManager
{
private:
	bool CheckBoxCollisionPair(Collider* colliderA, Collider* colliderB);
public:
	void CheckAllCollisions();
};