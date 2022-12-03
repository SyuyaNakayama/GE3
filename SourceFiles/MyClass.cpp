#include "MyClass.h"

ViewProjection::ViewProjection(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	view = XMMatrixIdentity();
	projection = XMMatrixIdentity();
	this->eye = eye;
	this->target = target;
	this->up = up;
}
void ViewProjection::CreateViewMatrix()
{
	view = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}
void ViewProjection::CreateProjectionMatrix(XMINT2 windowSize)
{
	projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f), (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
}

