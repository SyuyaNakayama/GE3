#include "PlayerBullet.h"

Model* PlayerBullet::model = nullptr;

void PlayerBullet::Initialize(Vector3 pos_, Vector3 spd_)
{
	if (!model) { model = Model::Create("playerBullet"); }
	worldTransform.translation = pos_;
	spd = spd_;
	worldTransform.Initialize();
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