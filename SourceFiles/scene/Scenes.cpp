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
	worldTransform.Initialize();
	worldTransformSmooth.Initialize();
	worldTransformSmooth.translation.x = 3.0f;
	skydome.Initialize();
	skydome.scale *= 10.0f;
	Sprite* skydomeSprite = modelSkydome->GetSprite();
	skydomeSprite->SetColor({ 0,0,0,1 });
	modelSkydome->TextureUpdate();
}

void LightScene::Update()
{
	AbstractScenes::Update();
	worldTransform.Update();
	worldTransformSmooth.Update();
	skydome.Update();
	ImGui::ShowDemoWindow();
	if (ImGui::CollapsingHeader("DirLight"))
	{
		for (size_t i = 0; i < 3; i++)
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
	//lightGroup->SetPointLightActive(0, true);
	//ImGuiManager::SliderVector("lightPos", pointLightpos, -10, 10);
	//ImGuiManager::ColorEditRGB("pointLight", pointLightColor);
	//ImGuiManager::SliderVector("lightAtten", pointLightAtten, -10, 10);
	//lightGroup->SetPointLightPos(0, pointLightpos);
	//lightGroup->SetPointLightColor(0, pointLightColor);
	//lightGroup->SetPointLightAtten(0, pointLightAtten);
}

void LightScene::Draw()
{
	Model::PreDraw();
	model->Draw(worldTransform);
	modelSmooth->Draw(worldTransformSmooth);
	modelSkydome->Draw(skydome);
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