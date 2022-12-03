#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>

using namespace std;

class Model
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// マテリアル
	struct Material
	{
		string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		float alpha;
		string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = {};
			specular = {};
			alpha = 1.0f;
		}
	};

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};
private:
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	vector<unsigned short> indices;
	// マテリアル
	Material material;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize=0;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	void LoadFromOBJInternal(const std::string& modelName);
public:
	static void StaticInitialize();
	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(const string& DIRECTORY_PATH, const string& FILENAME);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME);

	ID3D12GraphicsCommandList* GetCommandList() { return cmdList; }
	/// <summary>
	/// モデル作成
	/// </summary>
	void Initialize();

	void CreateBuffers();

	static Model* LoadFromOBJ(const std::string& modelName);

	void Draw(UINT rootParamIndexMaterial);
};