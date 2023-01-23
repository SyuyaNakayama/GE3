#include "CollisionPrimitive.h"
#include "MathUtility.h"
using namespace MathUtility;

void Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p[1] - p[0];
	Vector3 p0_p2 = p[2] - p[0];
	// �O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
	normal = p0_p1.cross(p0_p2);
	normal.normalize();
}