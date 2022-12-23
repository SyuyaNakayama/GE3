#include "CollisionManager.h"
#include <list>

using namespace std;

bool CollisionManager::CheckBoxCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!((UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask()) ||
		!((UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask()))
	{
		return false;
	}

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius() + colliderB->GetRadius();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

void CollisionManager::CheckAllCollisions()
{
	list<Collider*> colliders_;

	list<Collider*>::iterator itrA = colliders_.begin();

	Collider* colliderA = *colliders_.begin();
	list<Collider*>::iterator itrB = itrA;
	itrB++;

	for (; itrB != colliders_.end(); ++itrB)
	{
		Collider* colliderB = *itrB;
		if (CheckBoxCollisionPair(colliderA, colliderB))
		{
			colliderA->OnCollision(colliderB);
			colliderB->OnCollision(colliderA);
		}
	}
}