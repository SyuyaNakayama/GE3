#pragma once
#include "Collider.h"
#include "Model.h"
#include "Timer.h"
#include "EnemyBullet.h"

class Enemy : public Collider
{
private:
	static Model* model;
	static std::array<std::unique_ptr<Sprite>, 4> enemySprites;
	Sprite* sprite;
	bool isDead = true;
	Vector3 moveSpd;
	Timer moveTimer = 200;
	bool isMove = true;
	EnemyType type;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	Timer shotIntervel = 90;

	void CreateShot(Vector3 pos, Vector3 moveSpd, EnemyType enemyType);
	void Shot();
public:
	static void StaticInitialize();
	void Initialize(Vector3 pos, Vector3 moveSpd_, EnemyType enemyType);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
	void OnCollision(Collider* collider) { isDead = true; }
};

