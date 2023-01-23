#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ParticleManager.h"

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
}

void GamePlayScene::Update()
{
	viewProjection.Update();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	Model::PostDraw();
}