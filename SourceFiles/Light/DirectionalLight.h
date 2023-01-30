#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "Vector.h"

class DirectionalLight
{
private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static ID3D12Device* device;
	ComPtr<ID3D12Resource> constBuff;
	Vector3 lightdir = { 1,0,0 }; // ライト光線方向
	Vector3 lightcolor = { 1,1,1 }; // ライト色
	bool dirty = false; // ダーティフラグ
	bool active = false;
	void Initialize();
public:
	struct ConstBufferData
	{
		Vector3 lightv; // ライトへの方向を表すベクトル
		float pad;
		Vector3 lightcolor; // ライトの色
		UINT active;
	};

	static void StaticInitialize();
	static DirectionalLight* Create();
	void TransferConstBuffer();
	void Update();
	void Draw(UINT rootParameterIndex);
	void SetLightDir(const Vector3& lightdir);
	void SetLightColor(const Vector3& lightcolor);
	void SetActive(bool active_) { active = active_; }
	Vector3 GetLightDir() { return lightdir; }
	Vector3 GetLightColor() { return lightcolor; }
	bool IsActive() { return active; }
};