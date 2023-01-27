#include "MyGame.h"
#include "Model.h"
#include "ImGuiManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager_->SetNextScene(Scene::Sphere, false);
	Model::InitializeGraphicsPipeline();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();
}

void MyGame::Draw()
{
	dxCommon->PreDraw();
	sceneManager_->Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}