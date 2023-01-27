#pragma once
#include "WorldTransform.h"
#include <vector>
#include <array>

enum class CollisionAttribute
{
	Player = 0b1,
	Block = 0b1 << 1,
	LadderBlock = 0b1 << 2,
	MouseRay = 0b1 << 3,
	StagePlane = 0b1 << 4,
	All = -1
};

enum class CollisionMask
{
	Player = (int)CollisionAttribute::Block | (int)CollisionAttribute::LadderBlock,
	Block = (int)CollisionAttribute::Player,
	MouseRay = (int)CollisionAttribute::StagePlane | (int)CollisionAttribute::Block,
	StagePlane = (int)CollisionAttribute::MouseRay,
	All = -1
};

class BoxCollider;
class SphereCollider;
class PlaneCollider;
class PolygonCollider;
class RayCollider;
class IncludeCollider;

class BaseCollider
{
private:
	CollisionAttribute collisionAttribute_ = CollisionAttribute::All;
	CollisionMask collisionMask_ = CollisionMask::All;

protected:
	WorldTransform worldTransform;

public:
	virtual ~BaseCollider() = default;

	virtual void OnCollision(BoxCollider* boxCollider) {}
	virtual void OnCollision(SphereCollider* sphereCollider) {}
	virtual void OnCollision(PlaneCollider* boxCollider) {}
	virtual void OnCollision(PolygonCollider* sphereCollider) {}
	virtual void OnCollision(RayCollider* sphereCollider) {}
	virtual void OnCollision(IncludeCollider* sphereCollider) {}

	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }
	CollisionMask GetCollisionMask() { return collisionMask_; }
	void SetCollisionAttribute(CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(CollisionMask collisionMask) { collisionMask_ = collisionMask; }
};

class BoxCollider : public virtual BaseCollider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetRadius() { return worldTransform.scale; }
};

class IncludeCollider : public virtual BaseCollider
{
public:
	enum class Axis { X, Y, Z };

private:
	static float includeRadius;
	// 当たり判定を取るペアのtrueが少ないほうが計算に反映される
	std::array<bool, 3> isUseAxis = { true,true,true };

public:
	IncludeCollider();
	virtual ~IncludeCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	static float GetIncludeRadius() { return includeRadius; }
	void SetUseAxis(Axis axis, bool isUse) { isUseAxis[(size_t)axis] = isUse; }
	std::array<bool, 3> GetUseAxis() { return isUseAxis; }
};

class SphereCollider : public virtual BaseCollider
{
public:
	SphereCollider();
	virtual ~SphereCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual float GetRadius() { return worldTransform.scale.x; }
};

class PlaneCollider : public virtual BaseCollider
{
protected:
	// 基準法線
	Vector3 baseNormal = Vector3::MakeYAxis();
	float distance = 0;
	Vector3 inter;

public:
	PlaneCollider();
	virtual ~PlaneCollider();

	void SetInter(const Vector3& inter_) { inter = inter_; }
	void SetDistance(float distance_) { distance = distance_; }
	void SetRotation(const Vector3& rotation) { worldTransform.rotation = rotation; }
	void SetBaseNormal(const Vector3& baseNormal_) { baseNormal = baseNormal_; }
	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(worldTransform.rotation); }
	virtual Vector3* GetInter() { return &inter; }
	virtual float GetDistance() { return distance; }
};

class PolygonCollider : public virtual BaseCollider
{
protected:
	// 基準法線
	Vector3 baseNormal = Vector3::MakeYAxis();
	// 頂点は時計回り
	std::vector<Vector3> vertices;
	float distance = 0;

public:
	PolygonCollider();
	virtual ~PolygonCollider();

	void UpdateVertices();
	void ComputeDistance() { distance = Dot(GetNormal(), vertices[0]); }
	void ToPlaneCollider(PlaneCollider* planeCollider);
	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(worldTransform.rotation); }
	virtual void SetVertices();
	virtual std::vector<Vector3> GetVertices() { return vertices; }
};

class RayCollider : public virtual BaseCollider
{
public:
	// 基準レイ
	Vector3 baseRayDirection = Vector3::MakeZAxis();
	RayCollider();
	virtual ~RayCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetRayDirection() { return baseRayDirection * Matrix4::Rotate(worldTransform.rotation); }
};