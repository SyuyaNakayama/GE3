#include "Sprite.h"
#include "SpriteCommon.h"
#include "WindowsAPI.h"
#include "DirectXCommon.h"
#include <cassert>
using namespace Microsoft::WRL;

template<class T> void BufferMapping(ID3D12Resource** buff, T** map, UINT64 width)
{
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = width;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �o�b�t�@�̐���
	HRESULT result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(buff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = (*buff)->Map(0, nullptr, (void**)map);
	assert(SUCCEEDED(result));
}

void Sprite::Initialize()
{
#pragma region ���_�o�b�t�@
	vertices.push_back({ { 0.0f,   100.0f }, { 0.0f, 1.0f } }); // ����
	vertices.push_back({ { 0.0f,     0.0f }, { 0.0f, 0.0f } }); // ����
	vertices.push_back({ { 100.0f, 100.0f }, { 1.0f, 1.0f } }); // �E��
	vertices.push_back(vertices[2]); // �E��
	vertices.push_back(vertices[1]); // ����
	vertices.push_back({ { 100.0f,   0.0f }, { 1.0f, 0.0f } }); // �E��

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	ID3D12Resource* vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	BufferMapping<Vertex>(&vertBuff, &vertMap, sizeVB);

	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);
#pragma endregion
#pragma region �萔�o�b�t�@
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	BufferMapping<ConstBufferDataMaterial>(constBuffMaterial.GetAddressOf(),
		&constMapMaterial, (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff);
	constMapMaterial->color = Color(1, 1, 1, 1);

	BufferMapping<ConstBufferDataTransform>(constBuffTransform.GetAddressOf(),
		&constMapTransform, (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	matProj = OrthoGraphic(WindowsAPI::GetInstance()->WIN_SIZE);
#pragma endregion
}

void Sprite::Update()
{
	Matrix4 matRot, matTrans;
	matRot = RotateZ(rotation);
	position.x++;
	matTrans = Translate(VectorChange(position));

	matWorld = Identity();
	matWorld = matRot * matTrans;
	constMapTransform->mat = matWorld * matProj;
}

void Sprite::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	cmdList->SetPipelineState(spriteCommon->GetPipelineState());
	cmdList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	ID3D12DescriptorHeap* ppHeaps[] = { spriteCommon->GetSRVHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteCommon->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}