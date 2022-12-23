#pragma once
#include "Model.h"

class Skydome
{
private:
	WorldTransform worldTransform;
	std::unique_ptr<Model> model;
	float size = 100.0f;

public:
	void Initialize();
	void Draw();
};