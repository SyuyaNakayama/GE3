#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "Timer.h"

class ResultScene : public BaseScene
{
private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> textSprite;
	Scene scene;
	std::unique_ptr<Sprite> spaceKeyUI;
	Timer skuAnimeTimer = 30;
	size_t animeFrame = 0;

public:
	ResultScene(Scene scene_) { scene = scene_; };
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};