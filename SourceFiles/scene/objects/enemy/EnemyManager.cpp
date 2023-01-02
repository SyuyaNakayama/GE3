#include "EnemyManager.h"
using namespace std;

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
		CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		CreateEnemy({ -20,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		CreateEnemy({ 20,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		break;
	case 2:
		CreateEnemy({ 0,0,150 }, { 0.1f,0,-0.5 }, EnemyType::Green);
		CreateEnemy({ 0,0,150 }, { -0.1f,0,-0.5 }, EnemyType::Green);
		CreateEnemy({ 0,0,150 }, { 0,0,-0.5 }, EnemyType::Green);
		CreateEnemy({ 0,0,150 }, { 0.1f,0.05f,-0.5 }, EnemyType::Green);
		CreateEnemy({ 0,0,150 }, { -0.1f,0.05f,-0.5 }, EnemyType::Green);
		break;
	case 3:
		CreateEnemy({ -20,0,150 }, { 0,0,-0.5f }, EnemyType::Red);
		CreateEnemy({ 20,0,150 }, { 0,0,-0.5f }, EnemyType::Red);
		break;
	case 4:
		CreateEnemy({ -110,0,50 }, { 0.5f,0,0 }, EnemyType::Red);
		CreateEnemy({ -100,0,50 }, { 0.5f,0,0 }, EnemyType::Red);
		CreateEnemy({ -90,0,50 }, { 0.5f,0,0 }, EnemyType::Red);
		CreateEnemy({ -110,10,50 }, { 0.5f,0,0 }, EnemyType::Red);
		CreateEnemy({ -90,10,50 }, { 0.5f,0,0 }, EnemyType::Red);
		break;
	case 5:
		CreateEnemy({ -20,0,150 }, { 0,0,-0.5f }, EnemyType::Red);
		CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Yellow);
		CreateEnemy({ 20,0,150 }, { 0,0,-0.5f }, EnemyType::Red);
		break;
	case 6:
		CreateEnemy({ -110,0,50 }, { 0.5f,0,0 }, EnemyType::Red);
		CreateEnemy({ -100,0,50 }, { 0.5f,0,0 }, EnemyType::Green);
		CreateEnemy({ -90,0,50 }, { 0.5f,0,0 }, EnemyType::Red);
		CreateEnemy({ -110,10,50 }, { 0.5f,0,0 }, EnemyType::Yellow);
		CreateEnemy({ -90,10,50 }, { 0.5f,0,0 }, EnemyType::Yellow);
		break;
	case 7:
		CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Purple);
		CreateEnemy({ -20,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		CreateEnemy({ 20,0,150 }, { 0,0,-0.5f }, EnemyType::Green);
		break;
	case 8:
		CreateEnemy({ 0,0,150 }, { 0,0,-0.5f }, EnemyType::Purple);
		CreateEnemy({ -100,10,50 }, { 0.5f,0,0 }, EnemyType::Yellow);
		break;
	case 9:
		for (size_t y = 0; y < 2; y++) {
			for (size_t x = 0; x < 3; x++)
			{
				CreateEnemy({ (float)x * 10.0f - 10.0f,(float)y * 10.0f - 2.0f,150 }, { 0,0,-0.6f }, EnemyType::Green);
			}
		}
		CreateEnemy({ -10,-2,150 }, { 0,0,-0.5f }, EnemyType::Red);
		CreateEnemy({ 0,-2,150 }, { 0,0,-0.5f }, EnemyType::Yellow);
		CreateEnemy({ 10,-2,150 }, { 0,0,-0.5f }, EnemyType::Red);
		CreateEnemy({ -10,10,150 }, { 0,0,-0.5f }, EnemyType::Yellow);
		CreateEnemy({ 10,10,150 }, { 0,0,-0.5f }, EnemyType::Yellow);
		break;
	case 10:
		for (size_t y = 0; y < 3; y++) {
			for (size_t x = 0; x < 5; x++)
			{
				CreateEnemy({ (float)x * 10.0f - 20.0f,(float)y * 10.0f - 2.0f,150 }, { 0,0,-0.5f }, EnemyType::Red);
			}
		}
		break;
	case 11:
		for (size_t y = 0; y < 5; y++) {
			for (size_t x = 0; x < 2; x++)
			{
				CreateEnemy({ (float)x * 40.0f - 120.0f,(float)y * 4.0f - 2.0f,50 }, { 0.5f,0,0 }, EnemyType::Yellow);
			}
		}
		CreateEnemy({ 0,6,1050 }, { 0,0,-5 }, EnemyType::Purple);

		break;
	case 12:
		for (size_t z = 0; z < 10; z++)
		{
			CreateEnemy({ 0,6,150 + 4 * (float)z }, { 0,0,-0.5f }, EnemyType::Red);
		}
		break;
	case 13:
		for (size_t y = 0; y < 2; y++) {
			for (size_t x = 0; x < 3; x++)
			{
				CreateEnemy({ (float)x * 20.0f - 20.0f,(float)y * 15.0f - 2.0f,150 }, { 0,0,-0.5f }, EnemyType::Purple);
			}
		}
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