#pragma once
#include "Vector.h"

struct Matrix4
{
	float m[4][4]=
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	Matrix4 operator*=(const Matrix4& m2);
};

Matrix4 Identity();
Matrix4 Zero();
Matrix4 Scale(const Vector3& s);
Matrix4 RotateX(float angle);
Matrix4 RotateY(float angle);
Matrix4 RotateZ(float angle);
Matrix4 Rotate(const Vector3& r);
Matrix4 Translate(const Vector3& t);
Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);