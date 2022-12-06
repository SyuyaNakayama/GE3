#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"

class ViewProjection
{
private:
	Matrix4 matView, matProjection;

public:
	Vector3 eye = { 0, 0, -50.0f }, target, up = { 0, 1, 0 };
	float fovAngleY = 45.0f; // ��p(�x)
	// �A�X�y�N�g��(��{��WindowsAPI::WIN_SIZE�ɏ���)
	float aspectRatio = WindowsAPI::GetInstance()->WIN_SIZE.x / WindowsAPI::GetInstance()->WIN_SIZE.y;
	float nearZ = 0.1f, farZ = 1000.0f; // �f��͈�

	void Initialize();
	void Update();
	void CameraMove(const Vector3& move);
	const Matrix4& GetViewProjectionMatrix() const { return matView * matProjection; }
};
