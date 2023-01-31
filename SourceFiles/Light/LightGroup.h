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
		ColorRGB ambientColor;
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
	};

	ComPtr<ID3D12Resource> constBuff;
	ColorRGB ambientColor;
	DirectionalLight dirLights[DIR_LIGHT_NUM]{};
	bool dirty = false;
	ConstBufferData* constMap = nullptr;

	void TransferConstBuffer();
public:
	static LightGroup* Create();
	void Initialize();
	void Update();
	void Draw(UINT rootParameterIndex);
	void DefaultLightSetting();
	void SetAmbientColor(const ColorRGB& color) { ambientColor = color; dirty = true; }
	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(int index, const Vector3& lightDir);
	void SetDirLightColor(int index, const ColorRGB& lightcolor);
};