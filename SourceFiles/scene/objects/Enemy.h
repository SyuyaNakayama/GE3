#pragma once
#include "Collider.h"
#include "Model.h"

class Enemy : public Collider
{
private:
	static Model* model;
	Sprite* sprite;
	bool isDead = true;
	Vector3 moveSpd;

public:
	void Initialize(Vector3 pos, Vector3 moveSpd_, Sprite* sprite_);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
	void OnCollision(Collider* collider) { isDead = true; }
};

