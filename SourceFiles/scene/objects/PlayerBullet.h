#pragma once
#include "Collider.h"
#include "Model.h"
#include "Timer.h"

class PlayerBullet : public Collider
{
private:
	static Model* model;
	Vector3 pos, spd;
	bool isDead;
	Timer deathTimer = 180;

public:
	void Initialize(Vector3 pos_,Vector3 spd_);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
};

