#pragma once
#include "BaseScene.h"
#include "Model.h"

class GamePlayScene : public BaseScene
{
private:
	ViewProjection viewProjection;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};