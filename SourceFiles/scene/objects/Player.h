#pragma once
#include "Collider.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Input.h"

class Player : public Collider
{
private:
	Model* model;
	const Vector2 LIMIT_POS = { 20,18 };
	std::list <std::unique_ptr<PlayerBullet>> bullets;
	Input* input = Input::GetInstance();

	void Move();
	void Shot();
public:
	void Initialize();
	void Update();
	void Draw();
};