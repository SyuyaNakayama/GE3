#include "Enemy.h"
#include "ImGuiManager.h"
#include "SharePtr.h"
#include <assert.h>
using namespace std;

void Enemy::Initialize(Vector3 pos, Vector3 moveSpd_, EnemyType enemyType)
{
	SetCollisionAttribute(CollisionAttribute::Enemy);
	SetCollisionMask(CollisionMask::Enemy);
	sprite = Sprite::CreatePointer("enemy.png");
	sprite->SetTextureLeftTop({ (float)enemyType * 16, 0 });
	sprite->SetTextureSize({ 16,16 });
	model = Model::Create("cube");
	model->SetSprite(sprite);
	model->TextureUpdate();
	moveSpd = moveSpd_;
	isDead = false;
	worldTransform.Initialize();
	worldTransform.scale = { 2,2,2 };
	worldTransform.translation = pos;
	type = enemyType;
}

void Enemy::CreateShot(Vector3 spd)
{
	unique_ptr<EnemyBullet> newBullet = make_unique<EnemyBullet>();
	newBullet->Initialize(worldTransform.GetWorldPosition(), spd, type);
	bullets.push_back(move(newBullet));
}

void Enemy::Shot()
{
	if (!shotIntervel.CountDown()) { return; }

	Vector3 spd = { 0,0,-1.5f };
	const size_t WAY_NUM = 8;
	float angle = PI * 2.0f / (float)WAY_NUM;

	switch (type)
	{
	case EnemyType::Red:
		CreateShot(spd);
		return;
	case EnemyType::Yellow:
		spd = Normalize(SharePtr::GetPlayer()->GetWorldPosition() - worldTransform.GetWorldPosition());
		CreateShot(spd * 1.5f);
		return;
	case EnemyType::Purple:
		for (size_t i = 0; i < WAY_NUM; i++)
		{
			spd = { cosf(angle * (float)i),sinf(angle * (float)i) };
			spd *= 0.2f;
			spd.z = -0.75f;
			CreateShot(spd);
		}
		return;
	}
}

void Enemy::Update()
{
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