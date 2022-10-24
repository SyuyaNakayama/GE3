#include "MyClass.h"

ShaderBlob::ShaderBlob(const std::wstring fileName, const LPCSTR target, ID3DBlob* errorBlob)
{
	HRESULT result;

	std::wstring filePath = L"Resources/shaders/" + fileName;

	result = D3DCompileFromFile(
		filePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&blob, &errorBlob);

	if (FAILED(result)) {
		assert(0);
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
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
	HRESULT result = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&state));
	assert(SUCCEEDED(result));
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

void ShaderResourceView::SetHeapDesc()
{
	heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NumDescriptors = 2056;
}
void ShaderResourceView::CreateDescriptorHeap(ID3D12Device* device)
{
	HRESULT result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
	assert(SUCCEEDED(result));
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