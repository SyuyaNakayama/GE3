#pragma once
#include "Collider.h"
#include "Model.h"
#include "Timer.h"

enum class EnemyType { Green, Red, Yellow, Purple };

class EnemyBullet : public Collider
{
private:
	static std::unique_ptr<Model> model;
	Vector3 spd;
	bool isDead;
	Timer deathTimer = 180;
	EnemyType type;
	static std::array<std::unique_ptr<Sprite>, 3> shotSprites;
	Sprite* sprite;

public:
	static void StaticInitialize();
	void Initialize(Vector3 pos, Vector3 spd_, EnemyType enemyType);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
	void OnCollision(Collider* collider) { isDead = true; }
};