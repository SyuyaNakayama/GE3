#include "LightGroup.h"
#include <cassert>
#include "DirectXCommon.h"

void LightGroup::TransferConstBuffer()
{
	// 定数バッファのマッピング
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
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
			// ライトが向こうなら転送しない
			else { constMap->dirLights[i].active = 0; }
		}

		constBuff->Unmap(0, nullptr);
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

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	HRESULT result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

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

void LightGroup::SetDirLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetLightColor(lightcolor);
	dirty = true;

}
