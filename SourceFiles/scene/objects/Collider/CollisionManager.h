#pragma once
#include "Collider.h"
#include <list>

class CollisionManager final
{
private:
	std::list<Collider*> colliders_;

	CollisionManager() = default;
	bool CheckBoxCollisionPair(Collider* colliderA, Collider* colliderB);
public:
	static CollisionManager* GetInstance();
	CollisionManager(const CollisionManager& obj) = delete;

	void PushCollider(Collider* collider) { colliders_.push_back(collider); }
	void PopCollider(Collider* collider) { colliders_.remove(collider); }
	void CheckAllCollisions();
};