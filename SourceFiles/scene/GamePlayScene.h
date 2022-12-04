#pragma once
#include <vector>
#include "Sprite.h"
#include "BaseScene.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class GamePlayScene : public BaseScene
{
private:
	Model* model[2]{};
	WorldTransform worldTransforms[3];
	ViewProjection viewProjection;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};