#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <chrono>
#include "Vector2.h"
#include "WindowsAPI.h"

class DirectXCommon final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12DescriptorHeap> dsvHeap, rtvHeap;
	ComPtr<ID3D12Fence> fence;
	HRESULT result = S_OK;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
	UINT64 fenceVal = 0;
	std::chrono::steady_clock::time_point reference_;
	D3D12_VIEWPORT viewport{};

	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
	void InitializeFixFPS();
	void UpdateFixFPS();
	DirectXCommon() = default;
public:
	static DirectXCommon* GetInstance();
	DirectXCommon(const DirectXCommon& obj) = delete;
	void Initialize();
	void PreDraw();
	void PostDraw();

	ID3D12Device* GetDevice() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
	void SetViewport(Vector2 viewportSize = WindowsAPI::WIN_SIZE, Vector2 viewportLeftTop = {});
};