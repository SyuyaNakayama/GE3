#pragma once
#include "Matrix4.h"

class ViewProjection
{
private:
	Matrix4 matView, matProjection;

public:
	Vector3 eye, target, up;

	void Initialize();
	Matrix4 GetViewProjectionMatrix() { return matView * matProjection; }
};

