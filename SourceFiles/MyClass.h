#pragma once
#include <DirectXMath.h>
#include "Buffer.h"
#include <DirectXTex.h>
#include "struct.h"

using namespace DirectX;

class ViewProjection
{
private:
	XMMATRIX view, projection;

public:
	XMFLOAT3 eye, target, up;

	ViewProjection(XMFLOAT3 eye, XMFLOAT3 target = {}, XMFLOAT3 up = { 0,1,0 });
	void CreateViewMatrix();
	void CreateProjectionMatrix(Int2 windowSize);
	XMMATRIX GetViewProjectionMatrix() { return view * projection; }
};

class WorldTransform
{
protected:
	XMMATRIX matWorld, matScale, matRot, matTrans;

	void InitializeMatrix();
public:
	XMFLOAT3 scale, rot, trans;
	WorldTransform* parent = nullptr;

	WorldTransform();
	WorldTransform(XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans);
	void UpdateMatrix();
};

class Object3d :public ConstBuf, public WorldTransform
{
public:
	Object3d(ID3D12Device* device, Type type = Transform);
	void TransferMatrix(ViewProjection viewProjection);
};