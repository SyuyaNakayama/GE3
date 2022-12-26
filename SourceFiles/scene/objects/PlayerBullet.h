#pragma once
#include "Collider.h"
#include "Model.h"
#include "Timer.h"

class PlayerBullet : public Collider
{
private:
	static Model* model;
	Vector3 spd;
	bool isDead;
	Timer deathTimer = 180;

public:
	void Initialize(Vector3 pos, Vector3 spd_);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
	void OnCollision(Collider* collider) { isDead = true; }
};

