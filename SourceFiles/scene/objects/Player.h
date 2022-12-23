#pragma once
#include "Collider.h"
#include "Model.h"

class Player : public Collider
{
private:
	//Model* model;

public:
	void Initialize();
	void Update();
	void Draw();
};