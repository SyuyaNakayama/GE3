#include "PipelineManager.h"
#include "Functions.h"
#include "DirectXCommon.h"

void PipelineManager::CreatePipeline(ComPtr<ID3D12PipelineState>& pipelinestate, ComPtr<ID3D12RootSignature>& rootsignature)
{
	Result result;

	// グラフィックスパイプラインの流れを設定
	VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	if (gsBlob) { GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get()); }

	// サンプルマスク
	SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	BlendState.RenderTarget[0] = blenddesc;

	// 頂点レイアウトの設定
	InputLayout.pInputElementDescs = inputLayout.data();
	InputLayout.NumElements = (UINT)inputLayout.size();

	NumRenderTargets = 1;	// 描画対象は1つ
	RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(rootParams.size(), rootParams.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(this, IID_PPV_ARGS(&pipelinestate));
}

void PipelineManager::AddRootParameter(RootParamType paramType)
{
	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	
	CD3DX12_ROOT_PARAMETER rootParam{};
	switch (paramType)
	{
	case RootParamType::CBV:
		rootParam.InitAsConstantBufferView(shaderRegister++);
		break;
	case RootParamType::DescriptorTable:
		rootParam.InitAsDescriptorTable(1, &descriptorRange);
		break;
	}
	rootParams.push_back(rootParam);
}

void PipelineManager::AddInputLayout(LPCSTR semanticName, DXGI_FORMAT format)
{
	inputLayout.push_back(SetInputLayout(semanticName, format));
}

void PipelineManager::SetBlendDesc(D3D12_BLEND_OP blendOp, D3D12_BLEND SrcBlend, D3D12_BLEND DestBlend)
{
	blenddesc.BlendOp = blendOp;
	blenddesc.SrcBlend = SrcBlend;
	blenddesc.DestBlend = DestBlend;
}

void PipelineManager::LoadShaders(std::wstring vsShaderName, std::wstring psShaderName, std::wstring gsShaderName)
{
	LoadShader(&vsBlob, vsShaderName, "vs_5_0");
	LoadShader(&psBlob, psShaderName, "ps_5_0");
	if (!gsShaderName.empty()) { LoadShader(&gsBlob, gsShaderName, "gs_5_0"); }
}