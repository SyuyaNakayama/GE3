#include <DirectXTex.h>
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Functions.h"
#include <array>
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

string SpriteCommon::DEFAULT_TEXTURE_DIRECTORY_PATH = "Resources/";
ComPtr<ID3D12RootSignature> SpriteCommon::rootSignature;
ComPtr<ID3D12PipelineState> SpriteCommon::pipelineState;
ComPtr<ID3D12DescriptorHeap> SpriteCommon::srvHeap;
ID3D12GraphicsCommandList* SpriteCommon::cmdList = nullptr;

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon* spriteCommon = new SpriteCommon;
	return spriteCommon;
}

void SpriteCommon::Initialize()
{
	device = DirectXCommon::GetInstance()->GetDevice();
	cmdList = DirectXCommon::GetInstance()->GetCommandList();

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	LoadShader(&vsBlob, L"SpriteVS", "vs_5_0");
	LoadShader(&psBlob, L"SpritePS", "ps_5_0");

	// 頂点レイアウト
	vector<D3D12_INPUT_ELEMENT_DESC> inputLayout =
	{
		SetInputLayout("POSITION",DXGI_FORMAT_R32G32_FLOAT),
		SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob);
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	CD3DX12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータの設定
	std::array<CD3DX12_ROOT_PARAMETER, 3> rootParams{};
	rootParams[0].InitAsConstantBufferView(0);
	rootParams[1].InitAsDescriptorTable(1, &descriptorRange);
	rootParams[2].InitAsConstantBufferView(1);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(rootParams.size(), rootParams.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	HRESULT result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;

	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

size_t SpriteCommon::GetIncrementSize()
{
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	return (size_t)incrementSize * textureIndex_;
}

uint32_t SpriteCommon::LoadTexture(const std::string& FILE_NAME)
{
	// テクスチャの重複読み込みを検出
	for (uint32_t i = 0; i < textureIndex_; i++)
	{
		if (textures_[i].fileName.find(FILE_NAME) == string::npos) { continue; }
		return i;
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{}, mipChain{};

	string fullPath = DEFAULT_TEXTURE_DIRECTORY_PATH + FILE_NAME;

	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);
	// ワイド文字列に変換
	vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

	HRESULT result = LoadFromWICFile(wfilePath.data(), WIC_FLAGS_NONE, &metadata, scratchImg);
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

	CD3DX12_RESOURCE_DESC textureResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height,
		(UINT16)metadata.arraySize, (UINT16)metadata.mipLevels);

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textures_[textureIndex_].buffer));
	assert(SUCCEEDED(result));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);
		result = textures_[textureIndex_].buffer->WriteToSubresource((UINT)i, nullptr, img->pixels,
			(UINT)img->rowPitch, (UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		srvHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex_,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	device->CreateShaderResourceView(GetTextureBuffer(textureIndex_), &srvDesc, srvHandle);

	textures_[textureIndex_].fileName = FILE_NAME;

	textures_[textureIndex_].cpuHandle = srvHandle;

	textures_[textureIndex_].gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		srvHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex_,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	return textureIndex_++;
}

void SpriteCommon::SetTextureCommands(uint32_t index)
{
	cmdList->SetGraphicsRootDescriptorTable(1, textures_[index].gpuHandle);
}

void SpriteCommon::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	// デスクリプタヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}