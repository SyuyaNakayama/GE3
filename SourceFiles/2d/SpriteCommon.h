#pragma once
#include <array>
#include <string>
#include "DirectXCommon.h"

class SpriteCommon
{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct TextureData
	{
		std::string fileName;
		ComPtr<ID3D12Resource> buffer;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle;
	};

	static const size_t MAX_SRV_COUNT = 2056;
	// デフォルトディレクトリ
	static std::string DEFAULT_TEXTURE_DIRECTORY_PATH;
	ID3D12Device* device = nullptr;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステート
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	std::array<TextureData, MAX_SRV_COUNT> textures_;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	uint32_t textureIndex_ = 0;

	SpriteCommon() = default;
public:
	static SpriteCommon* GetInstance();
	void Initialize();
	uint32_t LoadTexture(const std::string& FILE_NAME);
	void SetTextureCommands(uint32_t index);
	void PreDraw();
	void PostDraw() {};
	void IncrementTextureIndex() { textureIndex_++; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(UINT index) { return textures_[index].gpuHandle; }
	ID3D12DescriptorHeap* GetDescriptorHeap() { return srvHeap.Get(); }
	size_t GetIncrementSize();
	ID3D12Resource* GetTextureBuffer(uint32_t index) const { return textures_[index].buffer.Get(); }
};