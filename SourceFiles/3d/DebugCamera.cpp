#include "DebugCamera.h"
#include "Input.h"
#include <cmath>
#include "ImGuiManager.h"
#include <imgui.h>

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
	Input* input = Input::GetInstance();

	if (input->IsInputMouse(Mouse::Left)) { ImGui::Text("click!"); }
	if (input->IsInputMouse(Mouse::Middle)) { ImGui::Text("click middle!"); }
	ImGui::Text("%d,%d,%d", input->GetMouseMove().lX, input->GetMouseMove().lY, input->GetMouseMove().lZ);
	//Vector3 cameraMoveSpd = 1;
	Vector3 cameraMoveSpd = Vector3(input->Move(DIK_D, DIK_A, 1.0f), input->Move(DIK_W, DIK_S, 1.0f));
	if (cameraMoveSpd.Length() == 0) { return; }
	cameraMoveSpd.Normalize();
	rotQ.w = 1.0f;
	cameraMoveSpd = Quaternion::RotateVector(cameraMoveSpd, rotQ);
	//viewProjection.up = Normalize(Cross(-initEyePos, cameraMoveSpd));
	Vector3 nAxis = Normalize(Cross(-initEyePos, cameraMoveSpd));
	angle = 0.04f;
	rotQ = Quaternion::MakeAxisAngle(-nAxis, angle);
	viewProjection.eye = Quaternion::RotateVector(viewProjection.eye, rotQ);
	viewProjection.up = Normalize(Cross(cameraMoveSpd ,-viewProjection.eye));
	viewProjection.Update();
	ImGuiManager::PrintVector("cameraMoveSpd", cameraMoveSpd);
	ImGuiManager::PrintVector("debugCamera.viewProjection.eye", viewProjection.eye);
	ImGuiManager::PrintVector("debugCamera.viewProjection.up", viewProjection.up);
	ImGui::Text("Dot(Normalize(cameraMoveSpd),Normalize(-initEyePos)) = %f", Dot(Normalize(cameraMoveSpd),Normalize(-initEyePos)));
}