#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "Vector.h"
#include "Color.h"

class DirectionalLight
{
private:
	Vector3 lightdir = { 1,0,0 }; // ���C�g��������
	ColorRGB lightcolor; // ���C�g�F
	bool active = false;

public:
	struct ConstBufferData
	{
		Vector3 lightv; // ���C�g�ւ̕�����\���x�N�g��
		float pad;
		ColorRGB lightcolor; // ���C�g�̐F
		UINT active;
	};

	void SetLightDir(const Vector3& lightdir);
	void SetLightColor(const ColorRGB& lightcolor);
	void SetActive(bool active_) { active = active_; }
	Vector3 GetLightDir() { return lightdir; }
	ColorRGB GetLightColor() { return lightcolor; }
	bool IsActive() { return active; }
};