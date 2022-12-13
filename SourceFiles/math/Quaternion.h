#pragma once
#include "Matrix4.h"

class Quaternion
{
private:
	float x = 0, y = 0, z = 0, // 虚部
		w = 0; // 実部
public:
	Quaternion(float w_, Vector3 iv) { x = iv.x, y = iv.y, z = iv.z, w = w_; }
	Quaternion(float w_ = 0, float x_ = 0, float y_ = 0, float z_ = 0) { x = x_, y = y_, z = z_; w = w_; }
	Vector3 GetImaginary() const { return { x,y,z }; }
	// 代入演算子オーバーロード
	void operator*=(const Quaternion& q);
	void operator/=(float norm) { x /= norm, y /= norm, z /= norm, w /= norm; }
	// 共役にする
	void Conjugate() { x = -x, y = -y, z = -z; }
	// このQuaternionのNormを返す
	float Norm() const;
	// 正規化する
	void Normalize() { *this /= Norm(); }
	// 逆Quaternionにする
	void Inverse();

	// 静的関数
	// メンバ変数の値を変えたくない時はこっちを使う
	// 単位Quaternion
	static Quaternion Identity() { return Quaternion(1, 0, 0, 0); }
	static Quaternion Conjugate(const Quaternion& q) { return Quaternion(q.w, -q.GetImaginary()); }
	static float Norm(const Quaternion& q) { return q.Norm(); }
	static Quaternion Normalize(const Quaternion& q);
	static Quaternion Inverse(const Quaternion& q);
};

Quaternion operator*(const Quaternion& q1, const Quaternion& q2); // 積
Quaternion operator/(const Quaternion& q1, float norm); // 積