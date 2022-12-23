#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "Quaternion.h"
#include <fstream>

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	model = Model::Create("cube");

	for (WorldTransform& w : worldTransforms)
	{
		w.Initialize();
		w.scale = { 7,7,7 };
	}
	worldTransforms[1].translation.z = -20.0f;
	viewProjection.Initialize();
	skydome.Initialize();
	sprite = Sprite::Create("mario.jpg");
}

void GamePlayScene::Update()
{
	Vector3 moveSpd =
	{
		input->Move(DIK_RIGHT,DIK_LEFT,1.0f),
		input->Move(DIK_UP,DIK_DOWN,1.0f), 0
	};

	viewProjection.CameraMove(moveSpd);

	WorldTransform::SetViewProjection(&viewProjection);
	viewProjection.Update();
	for (size_t i = 0; i < 2; i++)
	{
		worldTransforms[i].Update();
	}
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	model->Draw(worldTransforms[0]);
	model->Draw(worldTransforms[1], sprite.get());
	skydome.Draw();
	Model::PostDraw();
}