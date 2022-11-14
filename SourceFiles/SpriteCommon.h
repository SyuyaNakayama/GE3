#pragma once
#include "DirectXCommon.h"

class SpriteCommon
{
private:
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

public:
	void Initialize();
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
};