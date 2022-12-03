#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;

class ViewProjection
{
private:
	XMMATRIX view, projection;

public:
	XMFLOAT3 eye, target, up;

	ViewProjection(XMFLOAT3 eye, XMFLOAT3 target = {}, XMFLOAT3 up = { 0,1,0 });
	void CreateViewMatrix();
	void CreateProjectionMatrix(XMINT2 windowSize);
	XMMATRIX GetViewProjectionMatrix() { return view * projection; }
};

