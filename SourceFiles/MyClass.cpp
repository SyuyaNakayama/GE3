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
void ViewProjection::CreateProjectionMatrix(Int2 windowSize)
{
	projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f), (float)windowSize.width / windowSize.height, 0.1f, 1000.0f);
}

void WorldTransform::InitializeMatrix()
{
	matWorld = XMMatrixIdentity();
	matScale = XMMatrixIdentity();
	matRot = XMMatrixIdentity();
	matTrans = XMMatrixIdentity();
}
WorldTransform::WorldTransform()
{
	InitializeMatrix();
	scale = { 1.0f,1.0f,1.0f };
	rot = {};
	trans = {};
}
WorldTransform::WorldTransform(XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans)
{
	InitializeMatrix();
	this->scale = scale;
	this->rot = rot;
	this->trans = trans;
}
void WorldTransform::UpdateMatrix()
{
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixRotationZ(rot.z) * XMMatrixRotationX(rot.x) * XMMatrixRotationY(rot.y);
	matTrans = XMMatrixTranslation(trans.x, trans.y, trans.z);
	matWorld = matScale * matRot * matTrans;
	if (parent != nullptr) { matWorld *= parent->matWorld; }
}

Object3d::Object3d(ID3D12Device* device, Type type) :ConstBuf(type)
{
	SetResource(size, 1, D3D12_RESOURCE_DIMENSION_BUFFER);
	SetHeapProp(D3D12_HEAP_TYPE_UPLOAD); // ヒープ設定
	CreateBuffer(device);
	Mapping(); // 定数バッファのマッピング
}
void Object3d::TransferMatrix(ViewProjection viewProjection)
{
	mapTransform->mat = matWorld * viewProjection.GetViewProjectionMatrix();
}