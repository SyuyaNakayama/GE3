#include "TitleScene.h"
#include "SpriteCommon.h"

void TitleScene::Initialize()
{
	sprites.push_back(Sprite::Create("Mario.jpg"));
}

void TitleScene::Finalize()
{
	for (Sprite*& sprite : sprites) { delete sprite; }
}

void TitleScene::Update()
{
	for (Sprite*& sprite : sprites) { sprite->Update(); }
}

void TitleScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	for (Sprite*& sprite : sprites) { sprite->Draw(); }
	spriteCommon->PostDraw();
}