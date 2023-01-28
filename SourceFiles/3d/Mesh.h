#pragma once
#include "Sprite.h"
#include <d3dx12.h>
#include <vector>
#include <wrl.h>
#include <unordered_map>

/// <summary>
/// 形状データ
/// </summary>
class Mesh {
private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 頂点データ構造体
	struct VertexData
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
		Color color; // 色
	};

public: // メンバ関数

	/// <summary>
	/// 頂点データの追加
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	void AddVertex(const VertexData& vertex) { vertices.emplace_back(vertex); }

	/// <summary>
	/// 頂点インデックスの追加
	/// </summary>
	/// <param name="index">インデックス</param>
	void AddIndex(unsigned short index) { indices.emplace_back(index); }

	/// <summary>
	/// バッファの生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点バッファ取得
	/// </summary>
	/// <returns>頂点バッファ</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; }

	/// <summary>
	/// インデックスバッファ取得
	/// </summary>
	/// <returns>インデックスバッファ</returns>
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; }

	size_t GetVertexCount() { return vertices.size(); }

	void AddSmoothData(UINT16 indexPosition, UINT16 indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }

	void CalculateSmoothedVertexNormals();

private: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 頂点データ配列
	std::vector<VertexData> vertices;
	// 頂点インデックス配列
	std::vector<UINT16> indices;
	// 頂点バッファのマップ
	VertexData* vertMap = nullptr;
	// 頂点座標スムージング用データ
	std::unordered_map<UINT16, std::vector<UINT16>> smoothData;
};
