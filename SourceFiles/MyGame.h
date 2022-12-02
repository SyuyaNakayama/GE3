#pragma once
#include <memory>
#include "Framework.h"
#include "GamePlayScene.h"

class MyGame : public Framework
{
private:
	GamePlayScene* scene_ = nullptr;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};