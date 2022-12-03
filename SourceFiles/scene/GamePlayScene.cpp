#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "WindowsAPI.h"

void GamePlayScene::Initialize()
{
	model[0] = Model::LoadFromOBJ("triangle_mat");
	model[1] = Model::LoadFromOBJ("player");
	for (Object3d*& obj : object3d)
	{
		obj = Object3d::Create();
	}
	object3d[0]->SetModel(model[0]);
	object3d[1]->SetModel(model[1]);
	object3d[2]->SetModel(model[1]);

	for (WorldTransform& w : worldTransforms) { w.Initialize(); }
	worldTransforms[1].translation = { +25,0,+25 };
	worldTransforms[2].translation = { -25,0,-25 };
	viewProjection.eye.z = -25.0f;
	viewProjection.target.x = -1.0f;
	viewProjection.eye.x = -1.0f;
	viewProjection.Initialize();
}

void GamePlayScene::Finalize()
{
	for (size_t i = 0; i < 2; i++) { delete model[i]; }
	for (size_t i = 0; i < 3; i++) { delete object3d[i]; }
}

void GamePlayScene::Update()
{
	for (WorldTransform& w : worldTransforms) { w.Update(); }
}

void GamePlayScene::Draw()
{
	Object3d::PreDraw();
	for (size_t i = 0; i < 3; i++) { object3d[i]->Draw(worldTransforms[i],viewProjection); }
	Object3d::PostDraw();
}