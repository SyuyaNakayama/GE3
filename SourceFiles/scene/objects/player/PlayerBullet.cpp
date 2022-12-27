#include "PlayerBullet.h"

Model* PlayerBullet::model = nullptr;

void PlayerBullet::Initialize(Vector3 pos, Vector3 spd_)
{
	if (!model) { model = Model::Create("playerBullet"); }
	worldTransform.translation = pos;
	spd = spd_;
	worldTransform.Initialize();
	SetCollisionAttribute(CollisionAttribute::PlayerBullet);
	SetCollisionMask(CollisionMask::Player);
}

void PlayerBullet::Update()
{
	worldTransform.translation += spd;
	worldTransform.Update();
	if (deathTimer.CountDown()) { isDead = true; }
}

void PlayerBullet::Draw()
{
	model->Draw(worldTransform);
}