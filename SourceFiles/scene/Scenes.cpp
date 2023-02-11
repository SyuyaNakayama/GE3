#include "Scenes.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include <imgui.h>
#include "ImGuiManager.h"

ViewProjection AbstractScenes::viewProjection;
DebugCamera AbstractScenes::debugCamera;
LightGroup* AbstractScenes::lightGroup;

void LightScene::Initialize()
{
	modelSmooth = Model::Create("sphere", true);
	modelGround = Model::Create("ground", true);
	worldTransformSmooth.Initialize();
	skydome.Initialize(10.0f);
	ground.Initialize();
	ground.scale = { 10,1,5 };
	ground.translation.y = -2;
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->SetCircleShadowActive(0, true);
}

void LightScene::Update()
{
	ImGuiManager::DragVector("circleShadowDir", circleShadowDir);
	ImGuiManager::DragVector("circleShadowAtten", circleShadowAtten);
	ImGuiManager::DragVector("circleShadowFactorAngle", circleShadowFactorAngle);
	ImGuiManager::DragVector("spherePos", worldTransformSmooth.translation);

	lightGroup->SetCircleShadowDir(0, circleShadowDir);
	lightGroup->SetCircleShadowCasterPos(0, worldTransformSmooth.translation);
	lightGroup->SetCircleShadowAtten(0, circleShadowAtten);
	lightGroup->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);

	AbstractScenes::Update();
	worldTransformSmooth.Update();
	skydome.Update();
	ground.Update();
}

void LightScene::Draw()
{
	Model::PreDraw();
	modelSmooth->Draw(worldTransformSmooth);
	//skydome.Draw();
	modelGround->Draw(ground);
	Model::PostDraw();
}

void AbstractScenes::StaticInitialize()
{
	viewProjection.Initialize();
	debugCamera.Initialize({}, 15.0f);
	lightGroup = LightGroup::Create();
	WorldTransform::SetLightGroup(lightGroup);
	WorldTransform::SetViewProjection(&viewProjection);
	viewProjection.eye.z = -15.0f;
}

void AbstractScenes::Update()
{
	ImGui::Text("Q Key : UseDebugCamera");

	if (input->IsTrigger(DIK_Q))
	{
		isDebugMode = !isDebugMode;
		if (isDebugMode) { WorldTransform::SetViewProjection(&debugCamera.GetViewProjection()); }
		else { WorldTransform::SetViewProjection(&viewProjection); }
	}
	if (isDebugMode)
	{
		if (input->IsInput(DIK_A)) { debugCamera.Update(); }
		ImGui::Text("DebugMode : On");
	}
	else
	{
		viewProjection.Update();
		ImGui::Text("DebugMode : Off");
	}
	lightGroup->Update();
}