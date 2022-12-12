#include "ViewProjection.h"
#include <DirectXMath.h>
using namespace DirectX;

const XMFLOAT3& ChangeVec(const Vector3& v) { return XMFLOAT3(v.x, v.y, v.z); }
Matrix4 ChangeMat(const XMMATRIX& m)
{
	Matrix4 mat;

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++)
		{
			mat.m[i][j] = m.r[i].m128_f32[j];
		}
	}

	return mat;
}

void ViewProjection::Initialize() { Update(); }

void ViewProjection::Update()
{
	matProjection = ChangeMat(XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY),
		aspectRatio, nearZ, farZ));
	matView = ChangeMat(XMMatrixLookAtLH(XMLoadFloat3(&ChangeVec(eye)), XMLoadFloat3(&ChangeVec(target)), XMLoadFloat3(&ChangeVec(up))));
}

void ViewProjection::CameraMove(const Vector3& move)
{
	eye += move; target += move;
}