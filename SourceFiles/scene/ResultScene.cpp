#include "ResultScene.h"
#include "SceneManager.h"
#include "SpriteCommon.h"
#include "WindowsAPI.h"

void ResultScene::Initialize()
{
	sprite = Sprite::Create("models/skydome/Background.png");
	sprite->SetSize({ 1280,720 });
	sprite->Update();

	switch (scene)
	{
	case Scene::GameOver:
		textSprite = Sprite::Create("sceneSprites/GameOver.png");	break;
	case Scene::Clear:
		textSprite = Sprite::Create("sceneSprites/GameClear.png");	break;
	}
	textSprite->SetAnchorPoint({ 0.5f,0.5f });
	textSprite->SetPosition(WindowsAPI::WIN_SIZE / 2.0f - Vector2(0, 150.0f));
	textSprite->SetSize(textSprite->GetSize() * 2.0f);

	spaceKeyUI = Sprite::Create("ui/spaceKey.png");
	spaceKeyUI->SetAnchorPoint({ 0.5f,0.5f });
	spaceKeyUI->SetPosition(WindowsAPI::WIN_SIZE / 2.0f + Vector2(0, 150.0f));
	spaceKeyUI->SetTextureSize({ 128,64 });
	spaceKeyUI->SetSize({ 256,128 });
}

void ResultScene::Update()
{
	textSprite->Update();

	if (skuAnimeTimer.CountDown())
	{
		++animeFrame %= 2;
		Vector2 leftTop{};
		leftTop.x += 128.0f * animeFrame;
		spaceKeyUI->SetTextureLeftTop(leftTop);
	}

	spaceKeyUI->Update();

	if (input->IsTrigger(DIK_SPACE)) { sceneManager_->SetNextScene(Scene::Title); }
}

void ResultScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	sprite->Draw();
	textSprite->Draw();
	spaceKeyUI->Draw();
	spriteCommon->PostDraw();
}