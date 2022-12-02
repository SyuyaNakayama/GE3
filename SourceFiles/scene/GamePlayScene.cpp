#include "GamePlayScene.h"
#include "SpriteCommon.h"

void GamePlayScene::Initialize()
{
	sprites.push_back(Sprite::Create("Map.png"));
	sprites.push_back(Sprite::Create("reimu.png"));
	sprites.push_back(Sprite::Create("Map.png"));
	sprites.push_back(Sprite::Create("reimu.png"));

	for (size_t i = 0; i < 4; i++)
	{
		Vector2 pos = sprites[i]->GetPosition();
		pos = { 200.0f * (float)i,0 };
		sprites[i]->SetPosition(pos);
	}
}

void GamePlayScene::Finalize()
{
	for (Sprite*& sprite : sprites) { delete sprite; }
}

void GamePlayScene::Update()
{
	for (Sprite*& sprite : sprites) { sprite->Update(); }
}

void GamePlayScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	for (Sprite*& sprite : sprites) { sprite->Draw(); }
	spriteCommon->PostDraw();
}