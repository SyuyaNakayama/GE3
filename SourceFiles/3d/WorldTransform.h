#pragma once
#include "ViewProjection.h"
#include "LightGroup.h"

class WorldTransform
{
private:
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 viewproj; // ビュープロジェクション行列
		Matrix4 world; // ワールド行列
		Vector3 cameraPos; // カメラ座標(ワールド座標)
	};

	// ビュープロジェクションのポインタ
	static ViewProjection* viewProjection_;
	// ライト
	static std::unique_ptr<LightGroup> lightGroup;
public:
	Matrix4 matWorld;
	ConstBufferData* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
	Vector3 scale = { 1,1,1 }, rotation, translation;
	WorldTransform* parent = nullptr;

	void Initialize();
	void Update();
	Vector3 GetWorldPosition() { return { matWorld.m[3][0],matWorld.m[3][1],matWorld.m[3][2] }; }
	static void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	static void SetLightGroup(LightGroup* lightGroup) { WorldTransform::lightGroup.reset(lightGroup); }
	static ViewProjection* GetViewProjection() { return viewProjection_; }
	static LightGroup* GetLightGroup() { return lightGroup.get(); }
};