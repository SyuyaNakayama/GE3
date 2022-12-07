#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "WindowsAPI.h"
#include <fstream>
#include <cassert>

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
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

	audio = Audio::GetInatance();
	
	audio->Load(L"Resources/スノーマンライオット_蟲の知らせ_feat_初音ミク.mp3");
	audio->Play();
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
	audio->Finalize();
}