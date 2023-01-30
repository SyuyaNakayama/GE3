#pragma once
#include "DirectionalLight.h"

class LightGroup
{
public:
	static const int DIR_LIGHT_NUM = 3;

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct ConstBufferData
	{
		Vector3 ambientColor;
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
	};

	ComPtr<ID3D12Resource> constBuff;
	Vector3 ambientColor = { 1,1,1 };
	DirectionalLight dirLights[DIR_LIGHT_NUM]{};
	bool dirty = false;

	void TransferConstBuffer();
public:
	static LightGroup* Create();
	void Initialize();
	void Update();
	void Draw(UINT rootParameterIndex);
	void DefaultLightSetting();
	void SetAmbientColor(const Vector3& color) { ambientColor = color; dirty = true; }
	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(int index, const Vector3& lightDir);
	void SetDirLightColor(int index, const Vector3& lightcolor);
};