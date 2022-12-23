#include "Enemy.h"

void Enemy::Initialize(Vector3 moveSpd_, Sprite* sprite_)
{
	model = Model::Create("cube");
	sprite.reset(sprite_);
	moveSpd = moveSpd_;
	isDead = false;
	worldTransform.Initialize();
	sprite->Update();
}

void Enemy::Update()
{
	worldTransform.translation += moveSpd;
	worldTransform.Update();
}

void Enemy::Draw()
{
	model->Draw(worldTransform, sprite.get());
}