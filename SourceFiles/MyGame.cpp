#include "MyGame.h"
#include "scene/GamePlayScene.h"
#include "scene/TitleScene.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	BaseScene* scene = new TitleScene();
	sceneManager_->SetNextScene(scene);
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