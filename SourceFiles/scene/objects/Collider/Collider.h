#pragma once
#include "Vector.h"
#include "WorldTransform.h"

enum class CollisionAttribute
{
	Player = 0b1,
	Enemy = 0b1 << 1,
	All = -1
};

enum class CollisionMask
{
	Player = ~(int)CollisionAttribute::Player,
	Enemy = ~(int)CollisionAttribute::Enemy,
	All = -1
};

class Collider
{
private:
	CollisionAttribute collisionAttribute_ = CollisionAttribute::All;
	CollisionMask collisionMask_ = CollisionMask::All;
	Collider* p = nullptr;

protected:
	WorldTransform worldTransform;

public:
	Collider() { p = this; }

	virtual void OnCollision(Collider* collider) {};
	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); };
	virtual Vector3 GetRadius() { return worldTransform.scale; };

	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }
	CollisionMask GetCollisionMask() { return collisionMask_; }
	void SetCollisionAttribute(CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(CollisionMask collisionMask) { collisionMask_ = collisionMask; }
};