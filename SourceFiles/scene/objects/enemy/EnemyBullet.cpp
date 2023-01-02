#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Vector3 pos, Vector3 spd_, EnemyType enemyType)
{
	if (enemyType != EnemyType::Green)
	{
		sprite = Sprite::Create("enemyShot.png");
		sprite->SetTextureLeftTop(sprite->GetTextureLeftTop() + Vector2(((size_t)enemyType - 1) * 16, 0));
		sprite->SetTextureSize({ 16,16 });
	}
	model = Model::Create("cube");
	model->SetSprite(sprite.get());
	model->TextureUpdate();
	worldTransform.translation = pos;
	spd = spd_;
	worldTransform.Initialize();
	SetCollisionAttribute(CollisionAttribute::EnemyBullet);
	SetCollisionMask(CollisionMask::Enemy);
}

void EnemyBullet::Update()
{
	worldTransform.translation += spd;
	worldTransform.Update();
	if (deathTimer.CountDown()) { isDead = true; }
}

void EnemyBullet::Draw()
{
	model->Draw(worldTransform);
}