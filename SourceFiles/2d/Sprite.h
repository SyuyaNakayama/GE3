#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "Matrix4.h"
#include "VectorChange.h"

class Sprite
{
	struct Color { float r = 1, g = 1, b = 1, a = 1; };
	struct ConstBufferDataMaterial { Color color; };
	struct ConstBufferDataTransform { Matrix4 mat; };
	struct Vertex { Vector2 pos, uv; };
	enum VertexNumber
	{
		LB, // 左下
		LT, // 左上
		RB, // 右下
		RT  // 右上
	};
	// 頂点データ
	std::vector<Vertex> vertices;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial, constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	Vertex* vertMap = nullptr;
	Matrix4 matWorld;
	Matrix4 matProj;
	Vector2 position_;
	float rotation_ = 0;
	Color color_;
	Vector2 size_ = { 100.0f,100.0f };
	Vector2 anchorPoint_;
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isInvisible_ = false;

public:
	void Initialize();
	void Update();
	void Draw();

	void SetPosition(const Vector2& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	void SetColor(const Color& color) { color_ = color; }
	void SetSize(const Vector2& size) { size_ = size; }
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetIsFlipX(bool isFlipX = true) { isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY = true) { isFlipY_ = isFlipY; }
	void SetIsInvisible(bool isInvisible = true) { isInvisible_ = isInvisible; }

	const Vector2& GetPosition() const { return position_; }
	float GetRotation() const { return rotation_; }
	const Color& GetColor() const { return color_; }
	const Vector2& GetSize() const { return size_; }
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	bool GetIsFlipX() const { return isFlipX_; }
	bool GetIsFlipY() const { return isFlipY_; }
	bool GetIsInvisible() const { return isInvisible_; }
};