#pragma once
#include "Matrix4.h"

class ViewProjection
{
private:
	Matrix4 matView, matProjection;

public:
	Vector3 eye = { 0, 0, -50.0f }, target, up = { 0, 1, 0 };

	void Initialize();
	void Update();
	const Matrix4& GetViewProjectionMatrix() { return matView * matProjection; }
};
