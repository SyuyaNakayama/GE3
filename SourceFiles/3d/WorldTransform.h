#pragma once
#include "Matrix4.h"
#include <d3dx12.h>
#include <wrl.h>
#include "ViewProjection.h"

class WorldTransform
{
private:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Matrix4 mat;	// 3D�ϊ��s��
	};

	// �r���[�v���W�F�N�V�����̃|�C���^
	static ViewProjection* viewProjection_;
public:
	Matrix4 matWorld;
	ConstBufferData* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
	Vector3 scale = { 1,1,1 }, rotation, translation;
	WorldTransform* parent = nullptr;

	void Initialize();
	void Update();
	const Vector3& GetWorldTransration() { return { matWorld.m[3][0],matWorld.m[3][1],matWorld.m[3][2] }; }
	static void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
};