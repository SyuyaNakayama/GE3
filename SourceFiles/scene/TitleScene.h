#pragma once
#include <vector>
#include "Sprite.h"
#include "BaseScene.h"

class TitleScene : public BaseScene
{
private:
	std::vector<Sprite*> sprites{};

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};