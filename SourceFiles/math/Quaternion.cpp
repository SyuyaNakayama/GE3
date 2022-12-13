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