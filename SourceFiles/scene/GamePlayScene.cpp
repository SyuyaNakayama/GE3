#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	viewProjection.Initialize();
	viewProjection.eye = { 0,20,-30 };
	skydome.Initialize();
	player.Initialize();
	enemyManager.Initialize();
}

void GamePlayScene::Update()
{
	player.Update();
	enemyManager.Update();

	Vector3 cameraMoveSpd =
	{
		input->Move(DIK_D,DIK_A,1.0f),
		0,input->Move(DIK_W,DIK_S,1.0f)
	};

	viewProjection.CameraMove(cameraMoveSpd);
	viewProjection.Update();

	CollisionManager::GetInstance()->CheckAllCollisions();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	skydome.Draw();
	player.Draw();
	enemyManager.Draw();
	Model::PostDraw();
}