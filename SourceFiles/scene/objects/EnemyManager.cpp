#include "EnemyManager.h"
using namespace std;

void EnemyManager::Initialize()
{
	unique_ptr<Enemy> newEnemy = make_unique<Enemy>();
	enemySprites[0] = Sprite::Create("enemy.png");
	enemySprites[0]->SetTextureSize(enemySprites[0]->GetTextureSize() - Vector2(48, 0));
	newEnemy->Initialize({}, enemySprites[0].get());
	
	enemies[0].push_back(move(newEnemy));
}

void EnemyManager::Update()
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i].remove_if([](unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });

		for (unique_ptr<Enemy>& enemy : enemies[i])
		{
			enemy->Update();
		}
	}
}

void EnemyManager::Draw()
{
	for (size_t i = 0; i < enemies.size(); i++) {
		for (unique_ptr<Enemy>& enemy : enemies[i])
		{
			enemy->Draw();
		}
	}
}