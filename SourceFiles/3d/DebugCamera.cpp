#include "DebugCamera.h"
#include "Input.h"
#include <cmath>
#include "ImGuiManager.h"
#include <imgui.h>
#include "WindowsAPI.h"

void DebugCamera::Initialize(Vector3 targetPos, float distance_)
{
	distance = distance_;
	viewProjection.target = viewProjection.eye = targetPos;
	viewProjection.eye.z -= distance;
	initEyePos = viewProjection.eye;
	viewProjection.Initialize();
}

void DebugCamera::Update()
{
	bool dirty = false;
	Vector2 angle;
	Vector2 scale = { 1.0f / WindowsAPI::WIN_SIZE.x, 1.0f / WindowsAPI::WIN_SIZE.y };
	Input* input = Input::GetInstance();

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	// �}�E�X�̍��{�^����������Ă�����J��������]������
	if (input->IsInputMouse(Mouse::Left))
	{
		angle = -Vector2(mouseMove.lY * scale.x, mouseMove.lX * scale.y) * PI;
		dirty = true;
	}

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�������
	if (input->IsInputMouse(Mouse::Middle))
	{
		Vector3 move = Vector3(-mouseMove.lX, mouseMove.lY) / 100.0f;
		move = Quaternion::RotateVector(move,rotQ);

		viewProjection.CameraMove(move);
		dirty = true;
	}

	// �z�C�[�����͂ŋ�����ύX
	if (mouseMove.lZ != 0) 
	{
		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty) 
	{
		// �ǉ���]���̉�]�s��𐶐�
		Quaternion rotQNew;
		rotQNew = Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), -angle.y);
		rotQNew *= Quaternion::MakeAxisAngle(Vector3::MakeXAxis(), -angle.x);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		rotQ *= rotQNew;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		Vector3 vTargetEye = { 0.0f, 0.0f, -distance };
		Vector3 vUp = { 0.0f, 1.0f };

		// �x�N�g������]
		vTargetEye = Quaternion::RotateVector(vTargetEye,rotQ);
		vUp = Quaternion::RotateVector(vUp, rotQ);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		viewProjection.eye = viewProjection.target + vTargetEye;
		viewProjection.up = vUp;
	}
	viewProjection.Update();
}