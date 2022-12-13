#pragma once

class Vector2
{
public:
	float x = 0, y = 0;

	float Length() const; // ノルム(長さ)
	void Normalize(); // 正規化

	// 単項演算子オーバーロード
	Vector2 operator-() const { return Vector2(-x, -y); }

	// 代入演算子オーバーロード
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
};

class Vector3
{
public:
	float x = 0, y = 0, z = 0;

	Vector3(float x_ = 0, float y_ = 0, float z_ = 0) { x = x_, y = y_, z = z_; }
	Vector3(const Vector2& v, float z_ = 0) { x = v.x, y = v.y, z = z_; }

	float Length() const; // ノルム(長さ)
	Vector3 Normalize(); // 正規化

	// 単項演算子オーバーロード
	Vector3 operator-() const { return Vector3(-x, -y, -z); }

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

// 2項演算子オーバーロード
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

// 内積値
float Dot(const Vector2& v1, const Vector2& v2);
float Dot(const Vector3& v1, const Vector3& v2);
// 外積値
float Cross(const Vector2& v1, const Vector2& v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
// メンバ関数のグローバル関数版
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);

// 線形補間
const Vector3 Lerp(const Vector3& start, const Vector3& end, const float t);

Vector2 VectorChange(Vector3 vec);
Vector3 VectorChange(Vector2 vec);