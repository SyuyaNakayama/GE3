#include "Scenes.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "DirectXCommon.h"

ViewProjection AbstractScenes::viewProjection;
DebugCamera AbstractScenes::debugCamera;
LightGroup* AbstractScenes::lightGroup;

void LightScene::Initialize()
{
	model = Model::Create("sphere");
	modelSmooth = Model::Create("sphere", true);
	modelSkydome = Model::Create("background", true);
	modelGround = Model::Create("cube", true);
	worldTransform.Initialize();
	worldTransform.translation.x = -1.5f;
	worldTransformSmooth.Initialize();
	worldTransformSmooth.translation.x = 1.5f;
	skydome.Initialize();
	skydome.scale *= 10.0f;
	ground.Initialize();
	ground.scale = { 10,1,5 };
	ground.translation.y = -2;
	Sprite* skydomeSprite = modelSkydome->GetSprite();
	modelSkydome->TextureUpdate();
}

void LightScene::Update()
{
	AbstractScenes::Update();
	worldTransform.Update();
	worldTransformSmooth.Update();
	skydome.Update();
	ground.Update();
	if (ImGui::CollapsingHeader("DirLight"))
	{
		for (int i = 0; i < 3; i++)
		{
			std::string str = "dirLight" + std::to_string(i);
			ImGui::Checkbox(str.c_str(), dirLightActive + i);
			lightGroup->SetDirLightActive(i, dirLightActive[i]);
			if (!dirLightActive[i]) { continue; }
			ImGuiManager::SliderVector("dirLightDir" + std::to_string(i), dirLightDir[i], -10, 10);
			ImGuiManager::ColorEditRGB("dirLightColor" + std::to_string(i), dirLightColor[i]);
			lightGroup->SetDirLightDir(i, dirLightDir[i]);
			lightGroup->SetDirLightColor(i, dirLightColor[i]);
		}
	}

	if (ImGui::CollapsingHeader("PointLight"))
	{
		for (int i = 0; i < 3; i++)
		{
			std::string str = "pointLight" + std::to_string(i);
			ImGui::Checkbox(str.c_str(), pointLightActive + i);
			lightGroup->SetPointLightActive(i, pointLightActive[i]);
			if (!pointLightActive[i]) { continue; }
			ImGuiManager::SliderVector("lightPos" + std::to_string(i), pointLightpos[i], -10, 10);
			ImGuiManager::ColorEditRGB("pointLightColor" + std::to_string(i), pointLightColor[i]);
			ImGuiManager::SliderVector("lightAtten" + std::to_string(i), pointLightAtten[i], -10, 10);
			lightGroup->SetPointLightPos(i, pointLightpos[i]);
			lightGroup->SetPointLightColor(i, pointLightColor[i]);
			lightGroup->SetPointLightAtten(i, pointLightAtten[i]);
		}
	}
}

void LightScene::Draw()
{
	Model::PreDraw();
	model->Draw(worldTransform);
	modelSmooth->Draw(worldTransformSmooth);
	modelSkydome->Draw(skydome);
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
		debugCamera.Update();
		ImGui::Text("DebugMode : On");
	}
	else
	{
		viewProjection.Update();
		ImGui::Text("DebugMode : Off");
	}
	lightGroup->Update();
}