#include "Skydome.h"

void Skydome::Initialize()
{
	worldTransform.Initialize();
	worldTransform.scale = { SIZE, SIZE, SIZE };
	worldTransform.Update();
	model = Model::Create("skydome");
	Sprite* sprite= model->GetSprite();
	Vector2 spriteSize = sprite->GetTextureSize();
	spriteSize *= 4.0f;
	sprite->SetTextureSize(spriteSize);
	model->TextureUpdate();
}

void Skydome::Draw()
{
	worldTransform.Update(); 
	model->Draw(worldTransform);
}