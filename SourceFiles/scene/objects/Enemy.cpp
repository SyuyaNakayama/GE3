#include "Enemy.h"

Model* Enemy::model = nullptr;

void Enemy::Initialize(Vector3 pos, Vector3 moveSpd_, Sprite* sprite_)
{
	if (!model) { model = Model::Create("cube"); }
	sprite = sprite_;
	moveSpd = moveSpd_;
	isDead = false;
	worldTransform.Initialize();
	worldTransform.scale = { 2,2,2 };
	worldTransform.translation = pos;
}

void Enemy::Update()
{
	worldTransform.translation += moveSpd;
	model->TextureUpdate(sprite);
	worldTransform.Update();
}

void Enemy::Draw()
{
	model->Draw(worldTransform, sprite);
}