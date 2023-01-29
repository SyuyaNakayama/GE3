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
	if (input->IsInputMouse(Mouse::Left)) {
		Vector2 d =
		{ 
			mouseMove.lY * scale.x,
			mouseMove.lX * scale.y
		};

		angle = -d * PI;
		dirty = true;
	}

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�������
	if (input->IsInputMouse(Mouse::Middle)) {
		float dx = mouseMove.lX / 100.0f;
		float dy = mouseMove.lY / 100.0f;

		Vector3 move = { -dx, dy, };
		move = move * matRot;

		viewProjection.CameraMove(move);
		dirty = true;
	}

	// �z�C�[�����͂ŋ�����ύX
	if (mouseMove.lZ != 0) {
		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty) {
		// �ǉ���]���̉�]�s��𐶐�
		Matrix4 matRotNew;
		matRotNew *= Matrix4::RotateX(-angle.x);
		matRotNew *= Matrix4::RotateY(-angle.y);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		Vector3 vTargetEye = { 0.0f, 0.0f, -distance };
		Vector3 vUp = { 0.0f, 1.0f };

		// �x�N�g������]
		vTargetEye = vTargetEye * matRot;
		vUp = vUp * matRot;

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		viewProjection.eye = viewProjection.target + vTargetEye;
		viewProjection.up = vUp;
	}
	viewProjection.Update();
}