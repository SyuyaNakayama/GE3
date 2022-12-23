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

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};