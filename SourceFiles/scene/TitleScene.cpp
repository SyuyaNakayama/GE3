#include "TitleScene.h"
#include "SpriteCommon.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	sprite = Sprite::Create("sceneSprites/Title.png");
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
	sprite->Draw();
	spriteCommon->PostDraw();
}