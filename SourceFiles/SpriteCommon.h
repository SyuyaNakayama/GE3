#pragma once
#include "DirectXCommon.h"

class SpriteCommon
{
private:
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

public:
	void Initialize();
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
};