#pragma once
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <d3d12.h>

class Sprite
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMMATRIX = DirectX::XMMATRIX;
	
	struct ConstBufferDataMaterial
	{
		DirectX::XMFLOAT4 color;
	};

	struct ConstBufferDataTransform
	{
		XMMATRIX mat = DirectX::XMMatrixIdentity();
	};

	struct Vertex
	{
		XMFLOAT2 pos{};
		XMFLOAT2 uv{};
	};

	// 頂点データ
	std::vector<Vertex> vertices;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial, constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	XMMATRIX matProj = DirectX::XMMatrixIdentity();
	XMFLOAT2 position{};
	float rotation = 0;
public:
	void Initialize();
	void Update();
	void Draw();
};