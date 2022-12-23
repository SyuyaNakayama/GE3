#include "Skydome.h"

void Skydome::Initialize()
{
	worldTransform.Initialize();
	worldTransform.scale = { size, size, size };
	worldTransform.Update();
	model = Model::LoadFromOBJ("skydome");
}

void Skydome::Draw()
{
	model->Draw(worldTransform);
}