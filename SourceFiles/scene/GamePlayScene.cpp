#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include <fstream>
#include <cassert>

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	model = Model::LoadFromOBJ("cube");
	model2 = Model::LoadFromOBJ("cube");

	for (WorldTransform& w : worldTransforms)
	{
		w.Initialize();
		w.scale = { 7,7,7 };
	}
	worldTransforms[1].translation.z = -20.0f;
	viewProjection.Initialize();
}

void GamePlayScene::Update()
{
	Vector3 moveSpd =
	{
		input->Move(DIK_RIGHT,DIK_LEFT,1.0f),
		input->Move(DIK_UP,DIK_DOWN,1.0f), 0
	};

	Sprite* modelSprite = model->GetSprite();

	Sprite::Color color = modelSprite->GetColor();
	color.a += 0.01f;

	modelSprite->SetColor(color);
	model->TextureUpdate();

	viewProjection.CameraMove(moveSpd);

	WorldTransform::SetViewProjection(&viewProjection);
	viewProjection.Update();
	for (WorldTransform& w : worldTransforms) { w.Update(); }
}

void GamePlayScene::Draw()
{
	//DirectXCommon::GetInstance()->SetViewport();
	Model::PreDraw();
	model->Draw(worldTransforms[0]);
	model2->Draw(worldTransforms[1]);
	Model::PostDraw();
	//DirectXCommon::GetInstance()->SetViewport(WindowsAPI::WIN_SIZE / 2.0f, { WindowsAPI::WIN_SIZE.x / 2.0f,0 });
	//Model::PreDraw();
	//model->Draw(worldTransforms[0]);
	//model2->Draw(worldTransforms[1]);
	//Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	delete model;
}