#pragma once
#include "Collider.h"
#include "Model.h"
#include "Timer.h"

enum class EnemyType { Green, Red, Yellow, Purple };

class EnemyBullet : public Collider
{
private:
	std::unique_ptr<Model> model;
	std::unique_ptr<Sprite> sprite;
	Vector3 spd;
	bool isDead = false;
	Timer deathTimer = 180;

public:
	~EnemyBullet() { sprite.release(); };
	void Initialize(Vector3 pos, Vector3 spd_, EnemyType enemyType);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
	void OnCollision(Collider* collider) { isDead = true; }
};