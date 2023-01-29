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

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	// マウスの左ボタンが押されていたらカメラを回転させる
	if (input->IsInputMouse(Mouse::Left)) {
		Vector2 d =
		{ 
			mouseMove.lY * scale.x,
			mouseMove.lX * scale.y
		};

		angle = -d * PI;
		dirty = true;
	}

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (input->IsInputMouse(Mouse::Middle)) {
		float dx = mouseMove.lX / 100.0f;
		float dy = mouseMove.lY / 100.0f;

		Vector3 move = { -dx, dy, };
		move = move * matRot;

		viewProjection.CameraMove(move);
		dirty = true;
	}

	// ホイール入力で距離を変更
	if (mouseMove.lZ != 0) {
		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty) {
		// 追加回転分の回転行列を生成
		Matrix4 matRotNew;
		matRotNew *= Matrix4::RotateX(-angle.x);
		matRotNew *= Matrix4::RotateY(-angle.y);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		Vector3 vTargetEye = { 0.0f, 0.0f, -distance };
		Vector3 vUp = { 0.0f, 1.0f };

		// ベクトルを回転
		vTargetEye = vTargetEye * matRot;
		vUp = vUp * matRot;

		// 注視点からずらした位置に視点座標を決定
		viewProjection.eye = viewProjection.target + vTargetEye;
		viewProjection.up = vUp;
	}
	viewProjection.Update();
}