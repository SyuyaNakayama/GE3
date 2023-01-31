#include "LightGroup.h"
#include <cassert>
#include "DirectXCommon.h"
#include "Functions.h"

void LightGroup::TransferConstBuffer()
{	
	constMap->ambientColor = ambientColor;
	for (int i = 0; i < DIR_LIGHT_NUM; i++)
	{
		// ライトが有効なら設定を転送	
		if (dirLights[i].IsActive())
		{
			constMap->dirLights[i].active = 1;
			constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
			constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
		}
		// ライトが無効なら転送しない
		else { constMap->dirLights[i].active = 0; }
	}
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();
	instance->Initialize();
	return instance;
}

void LightGroup::Initialize()
{
	DefaultLightSetting();
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
	TransferConstBuffer();
}

void LightGroup::Update()
{
	if (!dirty) { return; }
	TransferConstBuffer();
	dirty = false;
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::DefaultLightSetting()
{
	for (DirectionalLight& dirLight : dirLights)
	{
		dirLight.SetActive(true);
		dirLight.SetLightColor({ 1,1,1 });
	}
	dirLights[0].SetLightDir({ 0,-1,0 });
	dirLights[1].SetLightDir({ 0.5f,0.1f,0.2f });
	dirLights[2].SetLightDir({ -0.5f,0.1f,-0.2f });
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const Vector3& lightDir)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetLightDir(lightDir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const ColorRGB& lightcolor)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetLightColor(lightcolor);
	dirty = true;
}
