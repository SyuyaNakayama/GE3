#include "Sprite.h"
using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
    this->spriteCommon = spriteCommon;
    dxCommon = DirectXCommon::GetInstance();
    HRESULT result;
    ID3D12Device* device = dxCommon->GetDevice();

    vertices.push_back({ -0.5f, -0.5f, 0.0f }); // 左下
    vertices.push_back({ -0.5f, +0.5f, 0.0f }); // 左上
    vertices.push_back({ +0.5f, -0.5f, 0.0f }); // 右下
    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * vertices.size());

    // 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
    // リソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // 頂点データ全体のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 頂点バッファの生成
    ID3D12Resource* vertBuff = nullptr;
    result = device->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(result));

    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    XMFLOAT3* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
    // 全頂点に対して
    for (int i = 0; i < vertices.size(); i++) {
        vertMap[i] = vertices[i]; // 座標をコピー
    }
    // 繋がりを解除
    vertBuff->Unmap(0, nullptr);

    // GPU仮想アドレス
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    // 頂点バッファのサイズ
    vbView.SizeInBytes = sizeVB;
    // 頂点1つ分のデータサイズ
    vbView.StrideInBytes = sizeof(XMFLOAT3);
}

void Sprite::Draw()
{
    ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
    
    // パイプラインステートとルートシグネチャの設定コマンド
    cmdList->SetPipelineState(spriteCommon->GetPipelineState());
    cmdList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
    // プリミティブ形状の設定コマンド
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
    // 頂点バッファビューの設定コマンド
    cmdList->IASetVertexBuffers(0, 1, &vbView);
    // 描画コマンド
    cmdList->DrawInstanced(vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}
