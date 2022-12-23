#pragma once
#include <vector>
#include "Sprite.h"
#include "BaseScene.h"

class TitleScene : public BaseScene
{
private:
	std::unique_ptr<Sprite> sprite;

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};