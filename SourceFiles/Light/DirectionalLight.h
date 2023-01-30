#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "Vector.h"

class DirectionalLight
{
private:
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static ID3D12Device* device;
	ComPtr<ID3D12Resource> constBuff;
	Vector3 lightdir = { 1,0,0 }; // ���C�g��������
	Vector3 lightcolor = { 1,1,1 }; // ���C�g�F
	bool dirty = false; // �_�[�e�B�t���O
	bool active = false;
	void Initialize();
public:
	struct ConstBufferData
	{
		Vector3 lightv; // ���C�g�ւ̕�����\���x�N�g��
		float pad;
		Vector3 lightcolor; // ���C�g�̐F
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