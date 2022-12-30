#include "GamePlayScene.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	WorldTransform::SetViewProjection(&viewProjection);
	viewProjection.Initialize();
	viewProjection.eye = { 0,20,-30 };
	skydome.Initialize();
	player.Initialize();
	enemyManager.Initialize();
	
	playerLifeGuage = Sprite::Create("white1x1.png");
	playerLifeGuage->SetPosition({ 320,650 });
	playerLifeHeart = Sprite::Create("ui/playerLifeHeart.png");
	playerLifeHeart->SetSize(playerLifeHeart->GetSize() * 2.0f);
	playerLifeHeart->SetPosition({ 240,650 });
	playerLifeHeart->Update();
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

	if (player.GetHp() <= 0) { sceneManager_->SetNextScene(Scene::GameOver); }
	if (enemyManager.IsGameClear()) { sceneManager_->SetNextScene(Scene::Clear); }

	float playerLifeRate = (float)player.GetHp() / Player::MAX_HP;
	playerLifeGuage->SetSize({ 23.0f * player.GetHp(),40 });
	playerLifeGuage->SetColor({ 1 - playerLifeRate,playerLifeRate,0,1 });
	playerLifeGuage->Update();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	skydome.Draw();
	player.Draw();
	enemyManager.Draw();
	Model::PostDraw();

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->PreDraw();
	playerLifeHeart->Draw();
	playerLifeGuage->Draw();
	spriteCommon->PostDraw();
}