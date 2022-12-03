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

	object3d[1]->SetPosition({ -25,0,-25 });
	object3d[2]->SetPosition({ +25,0,+25 });
}

void GamePlayScene::Finalize()
{
	for (size_t i = 0; i < 2; i++) { delete model[i]; }
	for (size_t i = 0; i < 3; i++) { delete object3d[i]; }
}

void GamePlayScene::Update()
{
	//object3d[0]->Update();
	for (Object3d* obj : object3d) { obj->Update(); }
}

void GamePlayScene::Draw()
{
	Object3d::PreDraw();
	//object3d[0]->Draw();
	for (Object3d* obj : object3d) { obj->Draw(); }
	Object3d::PostDraw();
}