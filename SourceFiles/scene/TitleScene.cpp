#include "TitleScene.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "ImGuiManager.h"

void TitleScene::Initialize()
{
	sprite = Sprite::Create("sceneSprites/Title.png");
	sprite->SetSize({ 1280,720 });
	sprite->Update();
	spaceKeyUI = Sprite::Create("ui/spaceKey.png");
	spaceKeyUI->SetPosition({ 740,520 });
	spaceKeyUI->SetTextureSize({ 128,64 });
	spaceKeyUI->SetSize({ 256,128 });
	audio->Load(L"playBGM.mp3");
	audio->Play();
	//audio->Load(L"shotSE.mp3");
}

void TitleScene::Update()
{
	audio->Play();
	if (skuAnimeTimer.CountDown())
	{
		++animeFrame %= 2;
		Vector2 leftTop{};
		leftTop.x += 128.0f * animeFrame;
		spaceKeyUI->SetTextureLeftTop(leftTop);
	}

	spaceKeyUI->Update();

	if (input->IsTrigger(DIK_SPACE)) { sceneManager_->SetNextScene(Scene::Play); }
}

void TitleScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	sprite->Draw();
	spaceKeyUI->Draw();
	spriteCommon->PostDraw();
}