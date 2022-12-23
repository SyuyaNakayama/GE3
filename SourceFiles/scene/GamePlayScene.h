#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Audio.h"
#include "Skydome.h"

class GamePlayScene : public BaseScene
{
private:
	std::unique_ptr<Model> model;
	WorldTransform worldTransforms[3];
	ViewProjection viewProjection;
	Color color;
	Skydome skydome;
	std::unique_ptr<Sprite> sprite;
public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};