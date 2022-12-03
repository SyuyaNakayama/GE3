#include "MyGame.h"
#include "scene/GamePlayScene.h"
#include "scene/TitleScene.h"
#include "Model.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	BaseScene* scene = new GamePlayScene();
	sceneManager_->SetNextScene(scene);
	Object3d::StaticInitialize();
	Model::StaticInitialize();
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