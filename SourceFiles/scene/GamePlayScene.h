#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Skydome.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ParticleManager.h"

class GamePlayScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	Skydome skydome;
	Player player;
	EnemyManager enemyManager;
	ParticleManager* particleManager = nullptr;

public:
	void Initialize() override;
	void Finalize();
	void Update() override;
	void Draw() override;
};