#pragma once
#include <array>
#include <wrl.h>
#include <d3d12.h>
#include "Matrix4.h"
#include "VectorChange.h"
#include <string>

class Sprite
{
public:
	struct Color
	{
	private:
		class ColorClass
		{
		private:
			float val_ = 1.0f;
		public:
			float operator=(float val);
			void operator+=(float val) { operator=(val_ + val); }
			void operator-=(float val) { operator=(val_ - val); }
		};
	public:
		ColorClass r, g, b, a;
		Color() {}
		Color(float red, float green, float blue, float alpha = 1.0f) { r = red, g = green, b = blue, a = alpha; }
	};

	enum VertexNumber
	{
		LB, // 左下
		LT, // 左上
		RB, // 右下
		RT  // 右上
	};
private:
	struct Vertex { Vector2 pos, uv; };
	struct ConstBufferDataMaterial { Color color; };
	struct ConstBufferDataTransform { Matrix4 mat; };

	const static Matrix4 matProj;

	std::array<Vertex, 4> vertices;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial, constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	Vertex* vertMap = nullptr;
	Matrix4 matWorld;
	Vector2 position_;
	float rotation_ = 0;
	Color color_;
	Vector2 size_; // 表示サイズ
	Vector2 anchorPoint_;
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isInvisible_ = false;
	uint32_t textureIndex_ = 0;
	Vector2 textureLeftTop_; // 切り取り領域の左上座標
	Vector2 textureSize_; // 切り取り領域のサイズ

	void AdjustTextureSize();
public:
	static Sprite* Create(const std::string& FILE_NAME);
	void Initialize(uint32_t textureIndex);
	void Update();
	void Draw();

	// setter
	void SetPosition(const Vector2& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	void SetColor(const Color& color) { color_ = color; }
	void SetSize(const Vector2& size) { size_ = size; }
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetIsFlipX(bool isFlipX = true) { isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY = true) { isFlipY_ = isFlipY; }
	void SetIsInvisible(bool isInvisible = true) { isInvisible_ = isInvisible; }
	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }
	void SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }
	// getter
	const Vector2& GetPosition() const { return position_; }
	float GetRotation() const { return rotation_; }
	const Color& GetColor() const { return color_; }
	const Vector2& GetSize() const { return size_; }
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	bool GetIsFlipX() const { return isFlipX_; }
	bool GetIsFlipY() const { return isFlipY_; }
	bool GetIsInvisible() const { return isInvisible_; }
	uint32_t GetTextureIndex() const { return textureIndex_; }
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }
	const Vector2& GetTextureSize() const { return textureSize_; }
	const Vector2& GetVerticesUv(VertexNumber vNum) { return vertices[vNum].uv; }
};