#pragma once
#include "Model.h"

class Skydome
{
private:
	WorldTransform worldTransform;
	Model* model;
	const float SIZE = 200.0f;

public:
	void Initialize();
	void Draw();
};