#include "Collision.h"
#include <math.h>
#include "MathUtility.h"
using namespace MathUtility;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋���
	float dist = sphere.center.dot(plane.normal);
	// ���ʂ̌��_���W�����Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	dist -= plane.distance;
	// �����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) { return false; }
	// �^����_���v�Z
	if (inter) { *inter = -dist * plane.normal + sphere.center; } // ���ʏ�̍ŋߐړ_���^����_�Ƃ���

	return true;
}

void Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p0_p1 = triangle.p[1] - triangle.p[0];
	Vector3 p0_p2 = triangle.p[2] - triangle.p[0];
	Vector3 p0_pt = point - triangle.p[0];

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p[0];
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p1_pt = point - triangle.p[1];

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p2.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p[1];
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p[0] + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.p[2];

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p[2];
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p[0] + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p[1] + w * (triangle.p[2] - triangle.p[1]);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p[0] + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	Vector3 v = p - sphere.center;
	float vLenSq = v.dot(v);
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (vLenSq > sphere.radius * sphere.radius) { return false; } // �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
	// �^����_���v�Z
	if (inter) { *inter = p; }
	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.center;
	float b = m.dot(ray.dir);
	float c = m.dot(m) - sphere.radius * sphere.radius;
	// ���C�̎n�_��sphere�̊O���ɂ���(c > 0)�A���C������Ă��������������Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c; // ���ʎ�
	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) { return false; }

	// ���C�͋��ƌ������Ă���
	float t = -b - sqrtf(discr); // ��������ŏ��̒lt���v�Z
	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t��0�ɃN�����v
	if (t < 0) { t = 0.0f; }

	if (distance) { *distance = t; }
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}