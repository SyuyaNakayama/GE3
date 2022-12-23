#pragma once
#include "Enemy.h"
#include <array>

class EnemyManager
{
private:
	std::array<std::list<std::unique_ptr<Enemy>>, 4> enemies;
	std::array<std::unique_ptr<Sprite>, 4> enemySprites;

public:
	void Initialize();
	void Update();
	void Draw();
};