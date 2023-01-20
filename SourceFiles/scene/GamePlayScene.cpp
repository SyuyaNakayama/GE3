#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	ParticleManager::StaticInitialize(WorldTransform::GetViewProjection());
}

void GamePlayScene::Update()
{
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	Model::PostDraw();
	particleManager->Draw();

	SpriteCommon::PreDraw();
	SpriteCommon::PostDraw();
}