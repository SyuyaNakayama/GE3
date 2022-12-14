#include "TitleScene.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "GamePlayScene.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "WindowsAPI.h"

using namespace ImGui;

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
	for (Sprite*& sprite : sprites) { sprite->Update(); }
	if (input->IsTrigger(DIK_RETURN))
	{
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Begin();

	Vector2 pos = sprites[0]->GetPosition();
	imguiManager->SliderVector("position", pos);
	sprites[0]->SetPosition(pos);
	Sprite::Color color = sprites[0]->GetColor();
	imguiManager->ColorEdit(color);
	sprites[0]->SetColor(color);
	ShowDemoWindow();

	imguiManager->End();
}

void TitleScene::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	for (Sprite*& sprite : sprites) { sprite->Draw(); }
	spriteCommon->PostDraw();
}