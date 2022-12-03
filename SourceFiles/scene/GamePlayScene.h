#pragma once
#include <vector>
#include "Sprite.h"
#include "BaseScene.h"
#include "Object3d.h"

class GamePlayScene : public BaseScene
{
private:
	Model* model[2]{};
	Object3d* object3d[3]{};
	WorldTransform worldTransforms[3];
	ViewProjection viewProjection;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};