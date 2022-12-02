#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <array>
#include <string>
#include "DirectXCommon.h"

class SpriteCommon
{
private:
	static const size_t MAX_SRV_COUNT = 2056;
	// デフォルトディレクトリ
	static std::string DEFAULT_TEXTURE_DIRECTORY_PATH;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, MAX_SRV_COUNT> textureBuffers_;
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	SpriteCommon() = default;
	size_t GetIncrementSize(uint32_t index);
public:
	static SpriteCommon* GetInstance();
	void Initialize();
	void LoadTexture(uint32_t index, const std::string& FILE_NAME);
	void SetTextureCommands(uint32_t index);
	void PreDraw();
	void PostDraw() {};
	ID3D12Resource* GetTextureBuffer(uint32_t index) const { return textureBuffers_[index].Get(); }
};