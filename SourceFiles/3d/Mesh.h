#pragma once
#include "Sprite.h"
#include <vector>
#include <unordered_map>

// 形状データ
class Mesh
{
public:
	// 頂点データ構造体
	struct VertexData
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
		ColorRGBA color; // 色
	};

private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// 頂点データ配列
	std::vector<VertexData> vertices;
	// 頂点インデックス配列
	std::vector<UINT16> indices;
	// 頂点バッファのマップ
	VertexData* vertMap = nullptr;
	//std::unique_ptr<VertexData> vertMap;
	// 頂点座標スムージング用データ
	std::unordered_map<UINT16, std::vector<UINT16>> smoothData;

public:
	// 頂点データの追加
	void AddVertex(const VertexData& vertex) { vertices.emplace_back(vertex); }
	void PopVertex() { vertices.pop_back(); }
	// 頂点インデックスの追加
	void AddIndex(unsigned short index) { indices.emplace_back(index); }
	void Update(Sprite* sprite, Vector2 spriteSizeRate);
	void Draw();
	// バッファの生成
	void CreateBuffers();
	size_t GetVertexCount() { return vertices.size(); }
	size_t GetIndexCount() { return indices.size(); }
	void AddSmoothData(UINT16 indexPosition, UINT16 indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }
	void CalculateSmoothedVertexNormals();
};
