#include <string>
#include <array>
#include <d3dcompiler.h>
#include <cassert>
#include <DirectXTex.h>
#include "SpriteCommon.h"
#include "SpriteCommon.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#pragma comment(lib, "d3dcompiler.lib")
using namespace std;
using namespace DirectX;

D3D12_INPUT_ELEMENT_DESC SetInputLayout(LPCSTR semanticName, DXGI_FORMAT format)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout =
	{
		semanticName, 0, format, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	return inputLayout;
}

ID3DBlob* LoadShader(wstring shaderName, LPCSTR target)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	wstring fileName = L"Resources/shaders/" + shaderName + L".hlsl";
	//�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result = D3DCompileFromFile(
		fileName.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&shaderBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		string error;
		error.resize(errorBlob->GetBufferSize());
		copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	return shaderBlob;
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon* spriteCommon = new SpriteCommon;
	return spriteCommon;
}

void SpriteCommon::Initialize()
{
	ID3DBlob* vsBlob = LoadShader(L"SpriteVS", "vs_5_0"); // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = LoadShader(L"SpritePS", "ps_5_0"); // �s�N�Z���V�F�[�_�I�u�W�F�N�g

	// ���_���C�A�E�g
	vector<D3D12_INPUT_ELEMENT_DESC> inputLayout =
	{
		SetInputLayout("POSITION",DXGI_FORMAT_R32G32_FLOAT),
		SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
	// �u�����h�X�e�[�g
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = inputLayout.size();
	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3]{};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor.ShaderRegister = 0;
	rootParams[0].Descriptor.RegisterSpace = 0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].Descriptor.ShaderRegister = 1;
	rootParams[2].Descriptor.RegisterSpace = 0;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	TexMetadata metadata{};
	ScratchImage scratchImg{}, mipChain{};
	result = LoadFromWICFile(L"Resources/Map.png", WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(),
		scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	metadata.format = MakeSRGB(metadata.format);

	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	ID3D12Resource* texBuff = nullptr;
	result = device->CreateCommittedResource(
		&textureHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);
		result = texBuff->WriteToSubresource((UINT)i, nullptr, img->pixels,
			(UINT)img->rowPitch, (UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	const size_t MAX_SRV_COUNT = 2056;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;

	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}