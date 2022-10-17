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
	ID3D12CommandList* commandLists;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	ComPtr<IDXGISwapChain4> swapchain;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	WindowsAPI* winApp = nullptr;

	ComPtr<ID3D12Fence> fence;
	UINT64 val;
	
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
public:
	void Initialize(WindowsAPI* winApp);
	void Update();
	void Draw();
};