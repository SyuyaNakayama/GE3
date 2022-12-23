#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Audio.h"
#include "Skydome.h"

class GamePlayScene : public BaseScene
{
private:
	Model* model = nullptr;
	Model* model2 = nullptr;
	WorldTransform worldTransforms[3];
	ViewProjection viewProjection;
	Sprite::Color color;
	Skydome skydome;
	Sprite* sprite = nullptr;
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};