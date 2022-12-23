#pragma once
#include "Collider.h"
#include "Model.h"

class Player : public Collider
{
private:
	std::unique_ptr<Model> model;

public:
	void Initialize();
	void Update();
	void Draw();
};