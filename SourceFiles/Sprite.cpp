#include "Sprite.h"
using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
    this->spriteCommon = spriteCommon;
    dxCommon = DirectXCommon::GetInstance();
    HRESULT result;
    ID3D12Device* device = dxCommon->GetDevice();

    vertices.push_back({ -0.5f, -0.5f, 0.0f }); // ����
    vertices.push_back({ -0.5f, +0.5f, 0.0f }); // ����
    vertices.push_back({ +0.5f, -0.5f, 0.0f }); // �E��
    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * vertices.size());

    // ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // ���_�o�b�t�@�̐���
    ID3D12Resource* vertBuff = nullptr;
    result = device->CreateCommittedResource(
        &heapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(result));

    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    XMFLOAT3* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
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
    vbView.StrideInBytes = sizeof(XMFLOAT3);
}

void Sprite::Draw()
{
    ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
    
    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
    cmdList->SetPipelineState(spriteCommon->GetPipelineState());
    cmdList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
    // �v���~�e�B�u�`��̐ݒ�R�}���h
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
    // ���_�o�b�t�@�r���[�̐ݒ�R�}���h
    cmdList->IASetVertexBuffers(0, 1, &vbView);
    // �`��R�}���h
    cmdList->DrawInstanced(vertices.size(), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}
