#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	ParticleManager::Initialize();
}

void GamePlayScene::Update()
{
	particleManager->Add({0,0,-20}, 60, 1, 0);
	particleManager->Update();
	viewProjection.Update();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	Model::PostDraw();
	particleManager->Draw();

	SpriteCommon::PreDraw();
	SpriteCommon::PostDraw();
}

void GamePlayScene::Finalize()
{
}