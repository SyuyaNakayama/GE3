#pragma once
#include "EnemyBullet.h"

class Enemy : public Collider
{
private:
	std::unique_ptr<Model> model;
	Sprite* sprite;
	bool isDead = true;
	Vector3 moveSpd;
	Timer moveTimer = 200;
	bool isMove = true;
	EnemyType type;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	Timer shotIntervel = 80;

	void CreateShot(Vector3 moveSpd);
	void Shot();
public:
	void Initialize(Vector3 pos, Vector3 moveSpd_, EnemyType enemyType);
	void Update();
	void Draw();
	bool IsDead() { return isDead; };
	void OnCollision(Collider* collider) { isDead = true; }
	void SetSprite(Sprite* sprite) { model->SetSprite(sprite); }
};

