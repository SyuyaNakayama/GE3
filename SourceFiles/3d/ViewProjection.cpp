#include "ViewProjection.h"
#include "WindowsAPI.h"
#include <DirectXMath.h>
using namespace DirectX;

const XMFLOAT3& ChangeVec(const Vector3& v) { return XMFLOAT3(v.x, v.y, v.z); }
const Matrix4& ChangeMat(const XMMATRIX& m) 
{
	Matrix4 mat;

	for (size_t i = 0; i < 4; i++){
		for (size_t j = 0; j < 4; j++)
		{
			mat.m[i][j] = m.r[i].m128_f32[j];
		}
	}

	return mat;
}

void ViewProjection::Initialize()
{
	Update();
	matProjection = ChangeMat(XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 
		WindowsAPI::GetInstance()->WIN_SIZE.x / WindowsAPI::GetInstance()->WIN_SIZE.y, 0.1f, 1000.0f));
}

void ViewProjection::Update()
{
	matView = ChangeMat(XMMatrixLookAtLH(XMLoadFloat3(&ChangeVec(eye)), XMLoadFloat3(&ChangeVec(target)), XMLoadFloat3(&ChangeVec(up))));
}