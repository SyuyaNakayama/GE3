#include "MyGame.h"
#include "Model.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager_->SetNextScene(Scene::Title, false);
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
	dxCommon->PostDraw();
}