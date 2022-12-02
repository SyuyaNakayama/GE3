#include "MyGame.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	//scene_ = new GamePlayScene();
	scene_ = new TitleScene();
	scene_->Initialize();
}

void MyGame::Finalize()
{
	scene_->Finalize();
	delete scene_;
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();
	scene_->Update();
}

void MyGame::Draw()
{
	dxCommon->PreDraw();
	scene_->Draw();
	dxCommon->PostDraw();
}