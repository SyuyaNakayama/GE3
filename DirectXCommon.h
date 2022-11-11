#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "MyClass.h"
#include "WindowsAPI.h"

class DirectXCommon
{
private:
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	HRESULT result;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;	
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_RESOURCE_BARRIER barrierDesc{};
	WindowsAPI* winApp = nullptr;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
public:
	void Initialize(WindowsAPI* winApp);
	void PreDraw();
	void PostDraw();

	ID3D12Device* GetDevice() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
};