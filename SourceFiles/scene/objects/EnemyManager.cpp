#include "EnemyManager.h"
using namespace std;

void EnemyManager::NewWave()
{
	unique_ptr<Enemy> newEnemy;

	switch (++wave)
	{
	case 1:
		newEnemy = make_unique<Enemy>();
		newEnemy->Initialize({0,0,50}, {}, enemySprites[0].get());

		enemies.push_back(move(newEnemy));
		break;
	}
}

void EnemyManager::Initialize()
{
	for (size_t i = 0; i < enemySprites.size(); i++)
	{
		enemySprites[i] = Sprite::Create("enemy.png");
		enemySprites[i]->SetTextureLeftTop(enemySprites[i]->GetTextureLeftTop() - Vector2(i * 16, 0));
		enemySprites[i]->SetTextureSize(enemySprites[i]->GetTextureSize() - Vector2(48, 0));
	}

	NewWave();
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