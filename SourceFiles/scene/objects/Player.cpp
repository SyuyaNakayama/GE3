#include "Player.h"

void Player::Initialize()
{
	worldTransform.Initialize();
	model = Model::Create("player");
	worldTransform.translation = { -50,0,-50 };
	ViewProjection* v = WorldTransform::GetViewProjection();
	v->target = worldTransform.translation + Vector3(0, 0, 20);
}

void Player::Update()
{
	worldTransform.Update();
}

void Player::Draw()
{
	model->Draw(worldTransform);
}