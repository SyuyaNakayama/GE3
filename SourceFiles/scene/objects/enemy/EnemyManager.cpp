#include "EnemyManager.h"
using namespace std;

void EnemyManager::Initialize()
{
	NewWave();
}

void EnemyManager::CreateEnemy(Vector3 pos, Vector3 moveSpd, EnemyType enemyType)
{
	unique_ptr<Enemy> newEnemy = make_unique<Enemy>();
	newEnemy->Initialize(pos, moveSpd, enemyType);
	enemies.push_back(move(newEnemy));
}

void EnemyManager::NewWave()
{
	switch (++wave)
	{
	case 1:
		CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Red);
		CreateEnemy({ -20,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		//CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Yellow);
		//CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Purple);
		//CreateEnemy({ 20,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		break;
	case 2:
		//CreateEnemy({ 0,0,150 }, { 0.1f,0,-0.5 }, EnemyType::Green);
		//CreateEnemy({ 0,0,150 }, { -0.1f,0,-0.5 }, EnemyType::Green);
		//CreateEnemy({ 0,0,150 }, { 0,0,-0.5 }, EnemyType::Green);
		//CreateEnemy({ 0,0,150 }, { 0.1f,0.05f,-0.5 }, EnemyType::Green);
		//CreateEnemy({ 0,0,150 }, { -0.1f,0.05f,-0.5 }, EnemyType::Green);
		break;
	}
}

void EnemyManager::Update()
{
	enemies.remove_if([](unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });
	if (enemies.empty()) { NewWave(); }

	for (unique_ptr<Enemy>& enemy : enemies) { enemy->Update(); }
}

void EnemyManager::Draw()
{
	for (unique_ptr<Enemy>& enemy : enemies) { enemy->Draw(); }
}