#include "TitleScene.h"
#include "SpriteCommon.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	sprites.push_back(Sprite::Create("Mario.jpg"));
	sprites[0]->SetPosition({ 100,100 });
}

void TitleScene::Finalize()
{
	for (Sprite*& sprite : sprites) { delete sprite; }
}

void TitleScene::Update()
{
	if (input->IsTrigger(DIK_RETURN))
	{
		sceneManager_->SetNextScene(Scene::Play);
	}
}

void TitleScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	for (Sprite*& sprite : sprites) { sprite->Draw(); }
	spriteCommon->PostDraw();
}