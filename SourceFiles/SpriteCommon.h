#pragma once
#include "DirectXCommon.h"

class SpriteCommon
{
private:
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	SpriteCommon() = default;
public:
	static SpriteCommon* GetInstance();
	void Initialize();
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap.Get(); }
};