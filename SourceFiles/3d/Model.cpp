#include "Model.h"
#include <fstream>
#include <sstream>
#include <DirectXTex.h>
#include <d3dx12.h>
#include "Functions.h"
#include "SpriteCommon.h"

using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ComPtr<ID3D12PipelineState> Model::pipelinestate = nullptr;
ComPtr<ID3D12RootSignature> Model::rootsignature = nullptr;
vector<Model*> Model::models = {};

void LoadVector3Stream(istringstream& stream, Vector3& vec)
{
	stream >> vec.x;
	stream >> vec.y;
	stream >> vec.z;
}

void Model::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	LoadShader(&vsBlob, L"OBJVertexShader", "vs_5_0");
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	LoadShader(&psBlob, L"OBJPixelShader", "ps_5_0");

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		SetInputLayout("POSITION",DXGI_FORMAT_R32G32B32_FLOAT),
		SetInputLayout("NORMAL",DXGI_FORMAT_R32G32B32_FLOAT),
		SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT),
		SetInputLayout("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT)
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));
}

Model* Model::LoadFromOBJ(const string& modelName)
{
	for (Model* model : models)
	{
		if (model->name == modelName) { return model; }
	}

	Model* model = new Model();
	model->LoadFromOBJInternal(modelName);
	model->CreateBuffers();
	models.push_back(model);
	return model;
}

void Model::SetSprite(Sprite* sprite_)
{
	isSpriteChange = sprite != sprite_;
	sprite = sprite_;
}

void Model::TextureUpdate()
{
	if (!isSpriteChange) { return; }
	sprite->Update();
	for (size_t i = 0; i < vertices.size(); i++)
	{
		Vector2 spriteUvLT = sprite->GetVerticesUv(Sprite::VertexNumber::LT);
		Vector2 spriteUvRB = sprite->GetVerticesUv(Sprite::VertexNumber::RB);
		Vector2 uv = vertices[i].uv;
		uv.x *= spriteUvRB.x;
		uv.y *= spriteUvRB.y;
		uv += spriteUvLT;
		vertMap[i].uv = uv;
		vertMap[i].color = sprite->GetColor();
	}
}

void Model::LoadFromOBJInternal(const std::string& modelName)
{
	ifstream file;
	name = modelName;
	const string FILENAME = modelName + ".obj",
		DIRECTORY_PATH = "Resources/models/" + modelName + "/";
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	vector<Vector3> positions, normals;
	vector<Vector2> texcoords;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		// �}�e���A���ǂݍ���
		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			LoadMaterial(DIRECTORY_PATH, filename);
		}
		// ���_���W�ǂݍ���
		if (key == "v")
		{
			Vector3 position{};
			LoadVector3Stream(line_stream, position);
			positions.emplace_back(position);
		}
		// �e�N�X�`�����W�ǂݍ���
		if (key == "vt")
		{
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}
		// �@���x�N�g���ǂݍ���
		if (key == "vn")
		{
			Vector3 normal{};
			LoadVector3Stream(line_stream, normal);
			normals.emplace_back(normal);
		}
		// �|���S���f�[�^�ǂݍ���
		if (key == "f")
		{
			string index_string;
			vector<VertexData> vert;
			while (getline(line_stream, index_string, ' '))
			{
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				VertexData vertex{};
				vertex.pos = positions[(size_t)indexPosition - 1];
				vertex.normal = normals[(size_t)indexNormal - 1];
				vertex.uv = texcoords[(size_t)indexTexcoord - 1];
				vertices.emplace_back(vertex);
				vert.emplace_back(vertex);
				indices.emplace_back(indices.size());

				if (vert.size() != 4) { continue; }
				vertices.pop_back();
				for (size_t j = 0; j < 4; j++)
				{
					if (j == 1) { continue; }
					vertices.push_back(vert[j]);
					indices.emplace_back(indices.size());
				}
			}
		}
	}
	file.close();
}

void Model::LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME)
{
	ifstream file;
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') { key.erase(key.begin()); }
		if (key == "newmtl") { line_stream >> material.name; }
		if (key == "Ka") { LoadVector3Stream(line_stream, material.ambient); }
		if (key == "Kd") { LoadVector3Stream(line_stream, material.diffuse); }
		if (key == "Ks") { LoadVector3Stream(line_stream, material.specular); }
		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;
			string path = DIRECTORY_PATH;
			path.erase(path.begin(), path.begin() + 10);
			sprite = Sprite::Create(path + material.textureFilename);
		}
	}
	file.close();
}

void Model::CreateBuffers()
{
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices.size());
	// ���_�o�b�t�@����
	BufferMapping(&vertBuff, &vertMap, sizeVB);
	// �S���_�ɑ΂���
	copy(vertices.begin(), vertices.end(), vertMap); // ���W���R�s�[
	vertBuff->Unmap(0, nullptr);
	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexData);

	unsigned short* indexMap = nullptr;
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// �C���f�b�N�X�o�b�t�@����
	BufferMapping(&indexBuff, &indexMap, sizeIB);
	// �S�C���f�b�N�X�ɑ΂���
	copy(indices.begin(), indices.end(), indexMap);	// �C���f�b�N�X���R�s�[
	indexBuff->Unmap(0, nullptr);
	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	ConstBufferData* constMap = nullptr;
	// �萔�o�b�t�@����
	BufferMapping(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	constMap->ambient = material.ambient;
	constMap->diffuse = material.diffuse;
	constMap->specular = material.specular;
	constMap->alpha = material.alpha;
	constBuffer->Unmap(0, nullptr);
}

void Model::PreDraw()
{
	// �R�}���h���X�g���Z�b�g
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Draw(const WorldTransform& worldTransform)
{
	Draw(worldTransform, sprite);
}

void Model::Draw(const WorldTransform& worldTransform, Sprite* sprite)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	cmdList->SetGraphicsRootConstantBufferView(0, worldTransform.constBuffer->GetGPUVirtualAddress());

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffer->GetGPUVirtualAddress());

	// �f�X�N���v�^�q�[�v�̔z��
	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	ID3D12DescriptorHeap* ppHeaps[] = { spCommon->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (!material.textureFilename.empty())
	{
		// �V�F�[�_���\�[�X�r���[���Z�b�g
		cmdList->SetGraphicsRootDescriptorTable(2, spCommon->GetGpuHandle(sprite->GetTextureIndex()));
	}
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}