#include "Enemy.h"
#include <assert.h>
using namespace std;

void Enemy::Initialize(Vector3 pos, Vector3 moveSpd_, EnemyType enemyType)
{
	SetCollisionAttribute(CollisionAttribute::Enemy);
	SetCollisionMask(CollisionMask::Enemy);
	sprite = Sprite::CreatePointer("enemy.png");
	sprite->SetTextureLeftTop(sprite->GetTextureLeftTop() + Vector2(((size_t)enemyType) * 16, 0));
	sprite->SetTextureSize({ 16,16 });
	model = Model::Create("cube");
	model->SetSprite(sprite);
	moveSpd = moveSpd_;
	isDead = false;
	worldTransform.Initialize();
	worldTransform.scale = { 2,2,2 };
	worldTransform.translation = pos;
	type = enemyType;
}

void Enemy::CreateShot(Vector3 pos, Vector3 spd, EnemyType enemyType)
{
	unique_ptr<EnemyBullet> newBullet = make_unique<EnemyBullet>();
	newBullet->Initialize(pos, spd, enemyType);
	bullets.push_back(move(newBullet));
}

void Enemy::Shot()
{
	if (!shotIntervel.CountDown()) { return; }

	switch (type)
	{
	case EnemyType::Red:
		CreateShot(worldTransform.GetWorldPosition(), { 0,0,-1.5f }, type);
		break;
	case EnemyType::Yellow:
		break;
	case EnemyType::Purple:
		break;
	}
}

void Enemy::Update()
{
	model->TextureUpdate(sprite);
	bullets.remove_if([](const std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	Shot();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) { bullet->Update(); }

	if (moveTimer.CountDown()) { isMove = false; }
	if (!isMove) { return; }
	worldTransform.translation += moveSpd;
	worldTransform.Update();
}

void Enemy::Draw()
{
	model->Draw(worldTransform);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) { bullet->Draw(); }
}