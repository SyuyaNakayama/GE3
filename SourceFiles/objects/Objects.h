#pragma once
#include "Collider.h"
#include "Model.h"

class Objects
{
protected:
	std::unique_ptr<Model> model;
	Vector3 moveSpd;

public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual ~Objects() {};
	void ChangeColor(Color color = { 1,0,0,1 });
	void SetMoveSpeed(const Vector3& spd) { moveSpd = spd; }
	virtual void SetPosition(const Vector3& pos) {}
};

class Sphere : public SphereCollider, public Objects
{
public:
	void Initialize();
	void Update();
	void Draw() { model->Draw(worldTransform); }
	void SetPosition(const Vector3& pos) { worldTransform.translation = pos; }
	void OnCollision(PlaneCollider* collider) { ChangeColor(); }
	void OnCollision(RayCollider* collider) { ChangeColor(); }
};

class Plane : public PlaneCollider, public Objects
{
public:
	void SetPosition(const Vector3& pos) { distance = pos.x; }
	void Initialize();
	void Update();
	void Draw() { model->Draw(worldTransform); }
	void OnCollision(RayCollider* collider) { ChangeColor(); }
	void OnCollision(SphereCollider* collider) { ChangeColor(); }
};

class Ray : public RayCollider, public Objects
{
private:
	WorldTransform child;
public:
	void SetPosition(const Vector3& pos) { worldTransform.translation = pos; }
	void Initialize();
	void Update();
	void Draw();
};

class Triangle : public PolygonCollider, public Objects
{
public:
	void Initialize();
	void Update();
	void Draw() { model->Draw(worldTransform); };
	void SetPosition(const Vector3& pos) { worldTransform.translation = pos; }
	void OnCollision(RayCollider* collider) { ChangeColor(); }
	void OnCollision(SphereCollider* collider) { ChangeColor(); }
};