#pragma once
#include <vector>
#include "Sprite.h"

class GamePlayScene
{
private:
	std::vector<Sprite*> sprites{};

public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
};