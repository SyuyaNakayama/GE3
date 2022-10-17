#include "DirectXCommon.h"
#include <cassert>
#include <vector>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize(WindowsAPI* winApp)
{
	assert(winApp);
	this->winApp = winApp;

	InitializeDevice();				// �f�o�C�X�̐���
	InitializeCommand();			// �R�}���h�֘A�̏�����
	InitializeSwapchain();			// �X���b�v�`�F�[���̏�����
	InitializeRenderTargetView();	// �����_�[�^�[�Q�b�g�r���[�̏�����
	InitializeDepthBuffer();		// �[�x�o�b�t�@�̏�����
	InitializeFence();				// �t�F���X�̏�����
}

void DirectXCommon::InitializeDevice()
{
#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}
	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel{};
	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
		}
	}

#ifdef _DEBUG
	// �G���[���Ƀu���[�N�𔭐�������
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif
}

void DirectXCommon::InitializeCommand()
{
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeSwapchain()
{
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	backBuffers.resize(swapchainDesc.BufferCount);

	ComPtr<IDXGISwapChain1> swapchain1;

	dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), winApp->GetHwnd(), &swapchainDesc, nullptr, nullptr,
		&swapchain1);

	swapchain1.As(&swapchain);
}

void DirectXCommon::InitializeRenderTargetView()
{
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
	UINT bbIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapchainDesc.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void DirectXCommon::InitializeDepthBuffer()
{
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WindowsAPI::WIN_WIDTH;
	depthResourceDesc.Height = WindowsAPI::WIN_HEIGHT;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

	ComPtr<ID3D12Resource> depthBuff = nullptr;
	device->CreateCommittedResource(
		&depthHeapProp, D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue, IID_PPV_ARGS(&depthBuff));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::InitializeFence()
{
	result = device->CreateFence(val, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));
}

void DirectXCommon::Update()
{
}

void DirectXCommon::Draw()
{
}