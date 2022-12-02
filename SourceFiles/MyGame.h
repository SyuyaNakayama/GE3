#pragma once
#include <memory>
#include "Framework.h"
#include "scene/GamePlayScene.h"
#include "scene/TitleScene.h"

class MyGame : public Framework
{
private:
	//GamePlayScene* scene_ = nullptr;
	TitleScene* scene_ = nullptr;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};