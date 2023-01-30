#include "Light.h"
#include "DirectXCommon.h"
#include <cassert>

ID3D12Device* Light::device = nullptr;

void Light::StaticInitialize()
{
	assert(!Light::device);
	Light::device = DirectXCommon::GetInstance()->GetDevice();
}

Light* Light::Create()
{
	Light* instance = new Light();
	instance->Initialize();
	return instance;
}

void Light::TransferConstBuffer()
{
	// 定数バッファのマッピング
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	constMap->lightv = -lightdir;
	constMap->lightcolor = lightcolor;
	constBuff->Unmap(0, nullptr);
}

void Light::Initialize()
{
	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	HRESULT result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void Light::Update()
{
	if (!dirty) { return; }
	TransferConstBuffer();
	dirty = false;
}

void Light::Draw(UINT rootParameterIndex)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void Light::SetLightDir(const Vector3& lightdir)
{
	this->lightdir = Normalize(lightdir);
	dirty = true;
}

void Light::SetLightColor(const Vector3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}