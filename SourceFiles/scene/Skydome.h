#pragma once
#include "Model.h"

class Skydome
{
private:
	WorldTransform worldTransform;
	Model* model = nullptr;
	float size = 100.0f;

public:
	void Initialize();
	void Draw();
	~Skydome() { delete model; }
};