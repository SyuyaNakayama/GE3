#pragma once
#include "Sprite.h"
#include <memory>
#include "Framework.h"

class MyGame : public Framework
{
private:
	// ゲームループで使う変数の定義
	std::vector<Sprite*> sprites{};

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};