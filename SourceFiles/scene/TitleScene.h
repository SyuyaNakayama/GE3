#pragma once
#include <vector>
#include "Sprite.h"
#include "BaseScene.h"
#include "Timer.h"
#include "Audio.h"

class TitleScene : public BaseScene
{
private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> spaceKeyUI;
	Timer skuAnimeTimer = 30;
	size_t animeFrame = 0;
	std::unique_ptr<Audio> bgm;

public:
	void Initialize() override;
	void Finalize() { bgm->Stop(); }
	void Update() override;
	void Draw() override;
};