#pragma once
#include "Vector2.h"
#include "Vector3.h"

struct Matrix4
{
	float m[4][4]=
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	const Matrix4& operator*=(const Matrix4& m2);
};

const Matrix4& Identity();
const Matrix4& Zero();
const Matrix4& Scale(const Vector3& s);
const Matrix4& RotateX(float angle);
const Matrix4& RotateY(float angle);
const Matrix4& RotateZ(float angle);
const Matrix4& Rotate(const Vector3& r);
const Matrix4& Translate(const Vector3& t);
const Matrix4& OrthoGraphic(const Vector2& windowSize);
const Matrix4& operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3& operator*(const Vector3& v, const Matrix4& m);