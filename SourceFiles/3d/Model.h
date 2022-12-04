#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include "Sprite.h"

using namespace std;

class Model
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
		XMFLOAT4 color; // 色
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
	VertexPosNormalUv* vertMap = nullptr;
	Sprite* sprite = nullptr;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
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
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize = 0;

	void LoadFromOBJInternal(const std::string& modelName);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME);

	void CreateBuffers();
public:
	/// <summary>
	/// モデル作成
	/// </summary>
	static Model* LoadFromOBJ(const std::string& modelName);
	
	void Draw();
};