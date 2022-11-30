#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "Matrix4.h"
#include "VectorChange.h"

class Sprite
{
	struct Color { float r = 0, g = 0, b = 0, a = 0; };

	struct ConstBufferDataMaterial
	{
		Color color;
	};

	struct ConstBufferDataTransform
	{
		Matrix4 mat;
	};

	struct Vertex
	{
		Vector2 pos{};
		Vector2 uv{};
	};

	// 頂点データ
	std::vector<Vertex> vertices;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial, constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	Matrix4 matWorld;
	Matrix4 matProj;
	Vector2 position{};
	float rotation = 0;
public:
	void Initialize();
	void Update();
	void Draw();
};