#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ParticleManager.h"

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	ParticleManager::Initialize();
}

void GamePlayScene::Update()
{
	AddParticleProp particleProp;
	particleProp.position = { 0,0,-20 };
	particleProp.posRange = 3.0f;
	particleProp.velRange = 0.05f;
	ParticleManager::Add(particleProp);
	ParticleManager::Update();
	viewProjection.Update();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	Model::PostDraw();
	ParticleManager::Draw();
}