#pragma once
#include "Enemy.h"
#include <array>

class EnemyManager
{
private:
	std::list<std::unique_ptr<Enemy>> enemies;
	std::array<std::unique_ptr<Sprite>, 4> enemySprites;
	UINT wave = 0;

	void NewWave();
public:
	void Initialize();
	void Update();
	void Draw();
};