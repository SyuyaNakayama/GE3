﻿#include "MyGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"
#include "Model.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	BaseScene* scene = new GamePlayScene();
	sceneManager_->Initialize();
	sceneManager_->SetNextScene(scene, false);
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