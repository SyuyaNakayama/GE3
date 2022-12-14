#include "TitleScene.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "GamePlayScene.h"
#include "ImGuiManager.h"

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
	if (input->IsTrigger(DIK_RETURN))
	{
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Begin();

	imguiManager->End();
}

void TitleScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	for (Sprite*& sprite : sprites) { sprite->Draw(); }
	spriteCommon->PostDraw();
}