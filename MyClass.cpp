#include "MyClass.h"

ShaderBlob::ShaderBlob(const LPCWSTR fileName, const LPCSTR target, ID3DBlob* errorBlob)
{
	HRESULT result;

	result = D3DCompileFromFile(
		fileName, // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&blob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

Pipeline::Pipeline()
{
	desc = {};
	state = nullptr;
}
void Pipeline::SetShader(ShaderBlob vs, ShaderBlob ps)
{
	desc.VS.pShaderBytecode = vs.blob->GetBufferPointer();
	desc.VS.BytecodeLength = vs.blob->GetBufferSize();
	desc.PS.pShaderBytecode = ps.blob->GetBufferPointer();
	desc.PS.BytecodeLength = ps.blob->GetBufferSize();
}
void Pipeline::SetInputLayout(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT layoutNum)
{
	desc.InputLayout.pInputElementDescs = inputLayout;
	desc.InputLayout.NumElements = layoutNum;
}
void Pipeline::SetOthers()
{
	// �T���v���}�X�N�̐ݒ�
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	desc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
	// �}�`�̌`��ݒ�
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// ���̑��̐ݒ�
	desc.NumRenderTargets = 1; // �`��Ώۂ�1��
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	desc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	desc.DepthStencilState.DepthEnable = true;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}
void Pipeline::CreatePipelineState(ID3D12Device* device)
{
	assert(SUCCEEDED(device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&state))));
}

RootSignature::RootSignature()
{
	params[0] = {};
	params[1] = {};
	desc = {};
	rs = nullptr;
	blob = nullptr;
}
void RootSignature::SetParam(D3D12_DESCRIPTOR_RANGE descriptorRange)
{
	for (UINT i = 0, j = 0; i < 3; i += 2)
	{
		params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
		params[i].Descriptor.ShaderRegister = j++;					// �萔�o�b�t�@�ԍ�
		params[i].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
		params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
	}

	params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	params[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	params[1].DescriptorTable.NumDescriptorRanges = 1;
	params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
}
void RootSignature::SetRootSignature(D3D12_STATIC_SAMPLER_DESC samplerDesc)
{
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.pParameters = params;
	desc.NumParameters = _countof(params);
	desc.pStaticSamplers = &samplerDesc;
	desc.NumStaticSamplers = 1;
}
void RootSignature::SerializeRootSignature(ID3D12Device* device, ID3DBlob* errorBlob)
{
	HRESULT result = D3D12SerializeRootSignature(&desc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, blob->GetBufferPointer(),
		blob->GetBufferSize(), IID_PPV_ARGS(&rs));
	assert(SUCCEEDED(result));
	blob->Release();
}

DirectXInit::DirectXInit()
{
	assert(SUCCEEDED(CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory))));
}
void DirectXInit::AdapterChoice()
{
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
}
ID3D12Device* DirectXInit::CreateDevice(D3D_FEATURE_LEVEL* levels, size_t levelsNum, ID3D12Device* device)
{
	HRESULT result;

	for (size_t i = 0; i < levelsNum; i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			return device;
		}
	}
	return nullptr;
}

RenderTargetView::RenderTargetView()
{
	rtvHeap = nullptr;
	rtvHeapDesc = {};
	rtvDesc = {};
	rtvHandle = {};
	devicePtr = nullptr;
}
void RenderTargetView::GetHandle()
{
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * devicePtr->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
}

SwapChain::SwapChain(ID3D12Device* device)
{
	scDesc.Width = 1280;
	scDesc.Height = 720;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	scDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	scDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	scDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	backBuffers.resize(scDesc.BufferCount);
	devicePtr = device;
	sc = nullptr;
}
void SwapChain::Create(IDXGIFactory7* dxgiFactory, ID3D12CommandQueue* commandQueue, HWND hwnd)
{
	ComPtr<IDXGISwapChain1> swapchain1;

	assert(SUCCEEDED(
		dxgiFactory->CreateSwapChainForHwnd(
			commandQueue, hwnd, &scDesc, nullptr, nullptr,
			&swapchain1)));

	swapchain1.As(&sc);
}
void SwapChain::CreateRenderTargetView()
{
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		sc->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += i * devicePtr->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		devicePtr->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}
void SwapChain::CreateDescriptorHeap()
{
	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = scDesc.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	devicePtr->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}
ID3D12Resource* SwapChain::GetBackBuffersPtr()
{
	bbIndex = sc->GetCurrentBackBufferIndex();
	return backBuffers[bbIndex].Get();
}

Blend::Blend(D3D12_RENDER_TARGET_BLEND_DESC* blenddesc)
{
	desc = blenddesc;
	desc->RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}
void Blend::UseBlendMode()
{
	desc->BlendEnable = true;
	desc->BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
	desc->SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
	desc->DestBlendAlpha = D3D12_BLEND_ZERO;	// �g��Ȃ�
}
void Blend::SetBlend(BlendMode blendMode)
{
	switch (blendMode)
	{
	case Blend::ADD:
		desc->BlendOp = D3D12_BLEND_OP_ADD;
		desc->SrcBlend = D3D12_BLEND_ONE;
		desc->DestBlend = D3D12_BLEND_ONE;
		break;
	case Blend::SUB:
		desc->BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		desc->SrcBlend = D3D12_BLEND_ONE;
		desc->DestBlend = D3D12_BLEND_ONE;
		break;
	case Blend::COLORFLIP:
		desc->BlendOp = D3D12_BLEND_OP_ADD;
		desc->SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		desc->DestBlend = D3D12_BLEND_ZERO;
		break;
	case Blend::ALPHA:
		desc->BlendOp = D3D12_BLEND_OP_ADD;
		desc->SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc->DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	}
}

ResourceBarrier::ResourceBarrier()
{
	desc = {};
	desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
}
void ResourceBarrier::SetState(ID3D12GraphicsCommandList* commandList)
{
	static int state = 0;
	if (!(state++))
	{
		desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		desc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}
	else
	{
		desc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		desc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}
	commandList->ResourceBarrier(1, &desc);
	state %= 2;
}

Command::Command(ID3D12Device* device)
{
	allocator = nullptr;
	list = nullptr;
	queue = nullptr;
	queueDesc = {};
	devicePtr = device;
}
void Command::CreateCommandAllocator()
{
	assert(SUCCEEDED(
		devicePtr->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&allocator))));
}
void Command::CreateCommandList()
{
	assert(SUCCEEDED(
		devicePtr->CreateCommandList(0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			allocator.Get(), nullptr,
			IID_PPV_ARGS(&list))));
}
void Command::CreateCommandQueue()
{
	assert(SUCCEEDED(
		devicePtr->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue))));
}
void Command::Reset()
{
	assert(SUCCEEDED(allocator->Reset())); // �L���[���N���A
	assert(SUCCEEDED(list->Reset(allocator.Get(), nullptr))); // �ĂуR�}���h���X�g�𒙂߂鏀��
}
void Command::ExecuteCommandLists()
{
	cLists = list.Get();
	queue->ExecuteCommandLists(1, &cLists);
}

Fence::Fence()
{
	f = nullptr;
	val = 0;
}
void Fence::CreateFence(ID3D12Device* device)
{
	assert(SUCCEEDED(device->CreateFence(val, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&f))));
}
void Fence::Wait()
{
	if (f->GetCompletedValue() != val)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		f->SetEventOnCompletion(val, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
}

void ShaderResourceView::SetHeapDesc()
{
	heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NumDescriptors = 2056;
}
void ShaderResourceView::CreateDescriptorHeap(ID3D12Device* device)
{
	assert(SUCCEEDED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap))));
}
void ShaderResourceView::GetDescriptorHandleForHeapStart(Type type)
{
	if (type == CPU) { handle = heap->GetCPUDescriptorHandleForHeapStart(); }
	if (type == GPU) { gpuHandle = heap->GetGPUDescriptorHandleForHeapStart(); }
}

ViewProjection::ViewProjection(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	view = XMMatrixIdentity();
	projection = XMMatrixIdentity();
	this->eye = eye;
	this->target = target;
	this->up = up;
}
void ViewProjection::CreateViewMatrix()
{
	view = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}
void ViewProjection::CreateProjectionMatrix(Int2 windowSize)
{
	projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f), (float)windowSize.width / windowSize.height, 0.1f, 1000.0f);
}

void WorldTransform::InitializeMatrix()
{
	matWorld = XMMatrixIdentity();
	matScale = XMMatrixIdentity();
	matRot = XMMatrixIdentity();
	matTrans = XMMatrixIdentity();
}
WorldTransform::WorldTransform()
{
	InitializeMatrix();
	scale = { 1.0f,1.0f,1.0f };
	rot = {};
	trans = {};
}
WorldTransform::WorldTransform(XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans)
{
	InitializeMatrix();
	this->scale = scale;
	this->rot = rot;
	this->trans = trans;
}
void WorldTransform::UpdateMatrix()
{
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixRotationZ(rot.z) * XMMatrixRotationX(rot.x) * XMMatrixRotationY(rot.y);
	matTrans = XMMatrixTranslation(trans.x, trans.y, trans.z);
	matWorld = matScale * matRot * matTrans;
	if (parent != nullptr) { matWorld *= parent->matWorld; }
}

Object3d::Object3d(ID3D12Device* device, Type type) :ConstBuf(type)
{
	SetResource(size, 1, D3D12_RESOURCE_DIMENSION_BUFFER);
	SetHeapProp(D3D12_HEAP_TYPE_UPLOAD); // �q�[�v�ݒ�
	CreateBuffer(device);
	Mapping(); // �萔�o�b�t�@�̃}�b�s���O
}
void Object3d::TransferMatrix(ViewProjection viewProjection)
{
	mapTransform->mat = matWorld * viewProjection.GetViewProjectionMatrix();
}