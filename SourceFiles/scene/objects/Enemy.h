#pragma once
#include "Collider.h"
#include "Model.h"

class Enemy : public Collider
{
private:
	std::unique_ptr<Model> model;
	std::unique_ptr<Sprite> sprite;
	bool isDead = true;
	Vector3 moveSpd;

public:
	void Initialize(Vector3 moveSpd_,Sprite* sprite_);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
};

