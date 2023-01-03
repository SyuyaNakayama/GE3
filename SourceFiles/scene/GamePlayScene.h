#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Audio.h"
#include "Skydome.h"
#include "Player.h"
#include "EnemyManager.h"

class GamePlayScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	Skydome skydome;
	Player player;
	EnemyManager enemyManager;
	std::unique_ptr<Sprite> playerLifeHeart, playerLifeGuage;
	std::unique_ptr<Audio> bgm;

public:
	void Initialize() override;
	void Finalize() { bgm->Stop(); bgm.release(); };
	void Update() override;
	void Draw() override;
};