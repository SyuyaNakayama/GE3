#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "Timer.h"
#include "Audio.h"

class ResultScene : public BaseScene
{
private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> textSprite;
	Scene scene;
	std::unique_ptr<Sprite> spaceKeyUI;
	Timer skuAnimeTimer = 30;
	size_t animeFrame = 0;
	std::unique_ptr<Audio> bgm;

public:
	ResultScene(Scene scene_) { scene = scene_; };
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};