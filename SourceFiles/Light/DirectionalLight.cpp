#include "DirectionalLight.h"
#include "DirectXCommon.h"
#include <cassert>

ID3D12Device* DirectionalLight::device;

void DirectionalLight::StaticInitialize()
{
	assert(!DirectionalLight::device);
	DirectionalLight::device = DirectXCommon::GetInstance()->GetDevice();
}

DirectionalLight* DirectionalLight::Create()
{
	DirectionalLight* instance = new DirectionalLight();
	instance->Initialize();
	return instance;
}

void DirectionalLight::TransferConstBuffer()
{
	// 定数バッファのマッピング
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	constMap->lightv = -lightdir;
	constMap->lightcolor = lightcolor;
	constBuff->Unmap(0, nullptr);
}

void DirectionalLight::Initialize()
{
	//// ヒーププロパティ
	//CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//// リソース設定
	//CD3DX12_RESOURCE_DESC resourceDesc =
	//	CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	//// 定数バッファの生成
	//HRESULT result = device->CreateCommittedResource(
	//	&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	//	IID_PPV_ARGS(&constBuff));
	//assert(SUCCEEDED(result));

	//TransferConstBuffer();
}

void DirectionalLight::Update()
{
	if (!dirty) { return; }
	TransferConstBuffer();
	dirty = false;
}

void DirectionalLight::Draw(UINT rootParameterIndex)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void DirectionalLight::SetLightDir(const Vector3& lightdir)
{
	this->lightdir = Normalize(lightdir);
	dirty = true;
}

void DirectionalLight::SetLightColor(const Vector3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}