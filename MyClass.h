#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <DirectXTex.h>
#include "Buffer.h"
#include "struct.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class ShaderBlob
{
public:
	ComPtr<ID3DBlob> blob = nullptr;

	ShaderBlob(const std::wstring fileName, const LPCSTR target, ID3DBlob* errorBlob);
};

class Pipeline
{
public:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ComPtr<ID3D12PipelineState> state;

	Pipeline();
	void SetShader(ShaderBlob vs, ShaderBlob ps);
	void SetInputLayout(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT layoutNum);
	void SetOthers();
	void CreatePipelineState(ID3D12Device* device);
};

class RootSignature
{
private:
	D3D12_ROOT_PARAMETER params[3];
	D3D12_ROOT_SIGNATURE_DESC desc;
	ComPtr<ID3DBlob> blob;
public:
	ComPtr<ID3D12RootSignature> rs;

	RootSignature();
	void SetParam(D3D12_DESCRIPTOR_RANGE descriptorRange);
	void SetRootSignature(D3D12_STATIC_SAMPLER_DESC samplerDesc);
	void SerializeRootSignature(ID3D12Device* device, ID3DBlob* errorBlob);
};

class Blend
{
private:
	D3D12_RENDER_TARGET_BLEND_DESC* desc;
public:
	enum BlendMode
	{
		ADD,
		SUB,
		COLORFLIP,
		ALPHA,
	};

	Blend(D3D12_RENDER_TARGET_BLEND_DESC* blenddesc);
	void UseBlendMode();
	void SetBlend(BlendMode blendMode);
};

class ShaderResourceView
{
private:
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
public:
	enum Type { CPU, GPU };

	ID3D12DescriptorHeap* heap;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE handle;

	void SetHeapDesc();
	void CreateDescriptorHeap(ID3D12Device* device);
	void GetDescriptorHandleForHeapStart(Type type);
};

class ViewProjection
{
private:
	XMMATRIX view, projection;

public:
	XMFLOAT3 eye, target, up;

	ViewProjection(XMFLOAT3 eye, XMFLOAT3 target = {}, XMFLOAT3 up = { 0,1,0 });
	void CreateViewMatrix();
	void CreateProjectionMatrix(Int2 windowSize);
	XMMATRIX GetViewProjectionMatrix() { return view * projection; }
};

class WorldTransform
{
protected:
	XMMATRIX matWorld, matScale, matRot, matTrans;

	void InitializeMatrix();
public:
	XMFLOAT3 scale, rot, trans;
	WorldTransform* parent = nullptr;

	WorldTransform();
	WorldTransform(XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans);
	void UpdateMatrix();
};

class Object3d :public ConstBuf, public WorldTransform
{
public:
	Object3d(ID3D12Device* device, Type type = Transform);
	void TransferMatrix(ViewProjection viewProjection);
};