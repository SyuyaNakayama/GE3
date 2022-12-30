#pragma once
#include "Enemy.h"

class EnemyManager
{
private:
	std::list<std::unique_ptr<Enemy>> enemies;
	UINT wave = 0;

	void CreateEnemy(Vector3 pos, Vector3 moveSpd, EnemyType enemyType);
	void NewWave();
public:
	void Initialize();
	void Update();
	void Draw();
	bool IsGameClear() { return wave >= 15; }
};