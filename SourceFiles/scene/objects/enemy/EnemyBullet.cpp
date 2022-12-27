#include "EnemyBullet.h"
#include <cassert>

std::array<std::unique_ptr<Sprite>, 3> EnemyBullet::shotSprites{};

void EnemyBullet::StaticInitialize()
{
	for (size_t i = 0; i < shotSprites.size(); i++)
	{
		shotSprites[i] = Sprite::Create("enemyShot.png");
		shotSprites[i]->SetTextureLeftTop(shotSprites[i]->GetTextureLeftTop() + Vector2(i * 16, 0));
		shotSprites[i]->SetTextureSize({ 16,16 });
	}
}

void EnemyBullet::Initialize(Vector3 pos, Vector3 spd_, EnemyType enemyType)
{
	if (enemyType != EnemyType::Green)
	{
		sprite = shotSprites[(size_t)enemyType - 1].get();
	}
	model = Model::Create("cube");
	model->SetSprite(sprite);
	model->TextureUpdate();
	worldTransform.translation = pos;
	spd = spd_;
	worldTransform.Initialize();
	SetCollisionAttribute(CollisionAttribute::EnemyBullet);
	SetCollisionMask(CollisionMask::Enemy);
	type = enemyType;
}

void EnemyBullet::Update()
{
	worldTransform.translation += spd;
	worldTransform.Update();
	if (deathTimer.CountDown()) { isDead = true; }
}

void EnemyBullet::Draw()
{
	if (sprite)
	{
		model->Draw(worldTransform);
	}
}