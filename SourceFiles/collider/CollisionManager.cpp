#include "CollisionManager.h"
#include <cassert>
#include <imgui.h>
#include <algorithm>
using namespace std;

list<BoxCollider*> CollisionManager::boxColliders;
list<IncludeCollider*> CollisionManager::includeColliders;
list<SphereCollider*> CollisionManager::sphereColliders;
list<PlaneCollider*> CollisionManager::planeColliders;
list<PolygonCollider*> CollisionManager::polygonColliders;
list<RayCollider*> CollisionManager::rayColliders;

bool CollisionManager::CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB)
{
	return
		(UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask() ||
		(UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask();
}

bool CollisionManager::CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius() + colliderB->GetRadius();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

bool CollisionManager::CheckCollision2IncludeObjects(IncludeCollider* colliderA, IncludeCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	// �g�p�t���O�̎擾
	array<bool, 3> isUse{};

	for (size_t i = 0; i < isUse.size(); i++)
	{
		isUse[i] = colliderA->GetUseAxis()[i] && colliderB->GetUseAxis()[i];
	}

	// �v�Z�Ɏg��Ȃ��l��0�ɂ���
	if (!isUse[(size_t)IncludeCollider::Axis::X]) { vecAB.x = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Y]) { vecAB.y = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Z]) { vecAB.z = 0; }

	return vecAB.Length() <= IncludeCollider::GetIncludeRadius();
}

bool CollisionManager::CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float radAB = colliderA->GetRadius() + colliderB->GetRadius();

	return vecAB.Length() <= radAB;
}

bool CollisionManager::CheckCollisionSpherePlane(SphereCollider* colliderA, PlaneCollider* colliderB, Vector3* inter)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋���
	float dist = Dot(colliderA->GetWorldPosition(), colliderB->GetNormal());
	// ���ʂ̌��_���W�����Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	dist -= colliderB->GetDistance();
	// �����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > colliderA->GetRadius()) { return false; }
	// �^����_���v�Z
	if (inter) { *inter = -dist * colliderB->GetNormal() + colliderA->GetWorldPosition(); } // ���ʏ�̍ŋߐړ_���^����_�Ƃ���

	return true;
}

bool CollisionManager::CheckCollisionSpherePolygon(SphereCollider* colliderA, PolygonCollider* colliderB, Vector3* inter)
{
	return false;
}

bool CollisionManager::CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl
	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = Dot(colliderB->GetNormal(), colliderA->GetRayDirection());
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return false; }
	// �n�_�ƌ��_�̋���(���ʂ̖@������)
	// �ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = Dot(colliderB->GetNormal(), colliderA->GetWorldPosition());
	// �n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - colliderB->GetDistance();
	// �n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	// ��_���n�_�����ɂ���̂œ�����Ȃ�
	if (t < 0) { return false; }
	// ��������������
	if (distance) { *distance = t; }
	// ��_���v�Z
	colliderB->SetInter(colliderA->GetWorldPosition() + t * colliderA->GetRayDirection());
	return true;
}

bool CollisionManager::CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	// �O�p�`������Ă��镽�ʂ��Z�o
	colliderB->ComputeDistance();
	// ���C�ƕ��ʂ��������Ă��Ȃ���Γ����Ă��Ȃ�
	PlaneCollider tempColliderB;
	colliderB->ToPlaneCollider(&tempColliderB);

	if (!CheckCollisionRayPlane(colliderA, &tempColliderB, distance)) { return false; }
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƍ��W���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl

	size_t vertexSize = colliderB->GetVertices().size();
	// ���_����2�ȉ��Ȃ�G���[
	assert(vertexSize > 2);

	for (size_t i = 0; i < vertexSize; i++)
	{
		// ��pi_p(i+1)�ɂ���
		Vector3 pt_px = colliderB->GetVertices()[i] - *tempColliderB.GetInter();
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = Cross(pt_px, px_py);
		// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
		if (Dot(m, colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// �����Ȃ̂œ������Ă���
	return true;
}

bool CollisionManager::CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance, Vector3* inter)
{
	Vector3 m = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float b = Dot(m, colliderA->GetRayDirection());
	float c = Dot(m, m) - colliderB->GetRadius() * colliderB->GetRadius();
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
	if (inter) { *inter = colliderA->GetWorldPosition() + t * colliderA->GetRayDirection(); }
	return true;
}

void CollisionManager::CheckBoxCollisions()
{
	for (BoxCollider* boxColliderA : boxColliders) {
		for (BoxCollider* boxColliderB : boxColliders)
		{
			if (boxColliderA == boxColliderB) { continue; }
			if (!CheckCollision2Boxes(boxColliderA, boxColliderB)) { continue; }

			boxColliderA->OnCollision(boxColliderB);
			boxColliderB->OnCollision(boxColliderA);
		}
	}
}

void CollisionManager::CheckIncludeCollisions()
{
	for (IncludeCollider* includeBoxColliderA : includeColliders) {
		for (IncludeCollider* includeBoxColliderB : includeColliders)
		{
			if (includeBoxColliderA == includeBoxColliderB) { continue; }
			if (!CheckCollision2IncludeObjects(includeBoxColliderA, includeBoxColliderB)) { continue; }

			includeBoxColliderA->OnCollision(includeBoxColliderB);
			includeBoxColliderB->OnCollision(includeBoxColliderA);
		}
	}
}

void CollisionManager::CheckSphereCollisions()
{
	for (SphereCollider* sphereColliderA : sphereColliders) {
		for (SphereCollider* sphereColliderB : sphereColliders)
		{
			if (sphereColliderA == sphereColliderB) { continue; }
			if (!CheckCollision2Spheres(sphereColliderA, sphereColliderB)) { continue; }

			sphereColliderA->OnCollision(sphereColliderB);
			sphereColliderB->OnCollision(sphereColliderA);
		}
	}
}

void CollisionManager::CheckRayPlaneCollisions()
{
	if (rayColliders.empty() || planeColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (PlaneCollider* planeCollider : planeColliders)
		{
			if (!CheckCollisionRayPlane(rayCollider, planeCollider)) { continue; }

			rayCollider->OnCollision(planeCollider);
			planeCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayPolygonCollisions()
{
	if (rayColliders.empty() || polygonColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (PolygonCollider* triangleCollider : polygonColliders)
		{
			if (!CheckCollisionRayPolygon(rayCollider, triangleCollider)) { continue; }

			rayCollider->OnCollision(triangleCollider);
			triangleCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckAllCollisions()
{
	CheckBoxCollisions();
	CheckRayPlaneCollisions();
	CheckRayPolygonCollisions();
	CheckIncludeCollisions();
	ImGui::Text("boxColliders.size() = %d", boxColliders.size());
	ImGui::Text("planeColliders.size() = %d", planeColliders.size());
	ImGui::Text("polygonColliders.size() = %d", polygonColliders.size());
	ImGui::Text("includeColliders.size() = %d", includeColliders.size());
}