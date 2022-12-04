#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "WindowsAPI.h"

void GamePlayScene::Initialize()
{
	model[0] = Model::LoadFromOBJ("triangle_mat");
	model[1] = Model::LoadFromOBJ("player");

	for (WorldTransform& w : worldTransforms)
	{
		w.Initialize();
		w.scale = { 20,20,20 };
	}
	worldTransforms[1].translation = { +25,0,+25 };
	worldTransforms[2].translation = { -25,0,-25 };
	viewProjection.Initialize();
}

void GamePlayScene::Update()
{
	WorldTransform::SetViewProjection(&viewProjection);
	for (WorldTransform& w : worldTransforms) { w.Update(); }
}

void GamePlayScene::Draw()
{
	Model::PreDraw();

	for (size_t i = 0; i < 2; i++) { model[i]->Draw(worldTransforms[i]); }
	model[1]->Draw(worldTransforms[2]);
	
	Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	for (size_t i = 0; i < 2; i++) { delete model[i]; }
}