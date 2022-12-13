#pragma once
#include "Vector.h"

class Matrix4
{
private:
	struct float4 { float x = 0, y = 0, z = 0, w = 0; };

public:
	float m[4][4] =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	Matrix4 operator*=(const Matrix4& m2);

	// �ÓI�֐�
	static Matrix4 Identity();
	static Matrix4 Zero();
	static Matrix4 Scale(const Vector3& s);
	static Matrix4 RotateX(float angle);
	static Matrix4 RotateY(float angle);
	static Matrix4 RotateZ(float angle);
	static Matrix4 Rotate(const Vector3& r);
	static Matrix4 Translate(const Vector3& t);
};

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);