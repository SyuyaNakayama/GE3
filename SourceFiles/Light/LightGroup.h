#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class LightGroup
{
public:
	static const int DIR_LIGHT_NUM = 3;
	static const int POINT_LIGHT_NUM = 3;

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct ConstBufferData
	{
		ColorRGB ambientColor;
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
		PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
	};

	ComPtr<ID3D12Resource> constBuff;
	ColorRGB ambientColor;
	DirectionalLight dirLights[DIR_LIGHT_NUM]{};
	PointLight pointLights[POINT_LIGHT_NUM]{};
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
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const Vector3& lightpos);
	void SetPointLightColor(int index, const ColorRGB& lightcolor);
	void SetPointLightAtten(int index, const Vector3& lightAtten);
};