#include "Quaternion.h"
#include <cmath>

void Quaternion::operator*=(const Quaternion& q)
{
	Vector3 iq1 = this->GetImaginary();
	Vector3 iq2 = q.GetImaginary();

	Quaternion ans =
	{
		this->w * q.w - Dot(iq1,iq2),
		Cross(iq1,iq2) + q.w * iq1 + this->w * iq2
	};
	*this = ans;
}

float Quaternion::Norm() const
{
	Vector3 i = GetImaginary();
	return sqrt(Dot(i, i) + w * w);
}

void Quaternion::Inverse()
{
	Conjugate();
	*this /= (Norm() * Norm());
}

Matrix4 Quaternion::MakeRotateMatrix() const
{
	Matrix4 mat =
	{
		w * w + x * x - y * y - z * z, 2 * (x * y + w * z), 2 * (x * z - w * y), 0,
		2 * (x * y - w * z), w * w - x * x + y * y - z * z, 2 * (y * z - w * x), 0,
		2 * (x * z + w * y), 2 * (y * z + w * x), w * w - x * x - y * y + z * z, 0,
		0,0,0,1
	};

	return mat;
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
	Quaternion ans = q;
	ans.Normalize();
	return ans;
}

Quaternion Quaternion::Inverse(const Quaternion& q)
{
	Quaternion ans = q;
	ans.Inverse();
	return ans;
}

Quaternion Quaternion::MakeAxisAngle(const Vector3& axis, float angle)
{
	return Quaternion(cosf(angle / 2.0f), axis * sinf(angle / 2.0f));
}

Vector3 Quaternion::RotateVector(const Vector3& v, const Quaternion& q)
{
	Quaternion r = { 0,v };
	return Quaternion(q * r * Conjugate(q)).GetImaginary();
}

Matrix4 Quaternion::MakeRotateMatrix(const Quaternion& q)
{
	return q.MakeRotateMatrix();
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion ans = q1;
	ans *= q2;
	return ans;
}

Quaternion operator/(const Quaternion& q1, float norm)
{
	Quaternion ans = q1;
	ans /= norm;
	return ans;
}