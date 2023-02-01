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
	model = Model::Create("sphere",true);
	modelSmooth = Model::Create("sphere");
	worldTransform.Initialize();
	worldTransformCube.Initialize();
	worldTransformCube.translation.x = 3.0f;
}

void LightScene::Update()
{
	AbstractScenes::Update();
	worldTransform.Update();
	worldTransformCube.Update();
	ImGui::Text("---------------------");
	for (size_t i = 0; i < 3; i++)
	{
		//lightGroup->SetDirLightActive(i, false);
		ImGuiManager::SliderVector("lightDir" + std::to_string(i), lightDir[i], -10, 10);
		ImGuiManager::ColorEditRGB("light" + std::to_string(i), lightColor[i]);
		lightGroup->SetDirLightDir(i, lightDir[i]);
		lightGroup->SetDirLightColor(i, lightColor[i]);
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
	modelSmooth->Draw(worldTransformCube);
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
	if (input->IsTrigger(DIK_1)) { sceneManager_->SetNextScene(Scene::Sprite); }
	if (input->IsTrigger(DIK_2)) { sceneManager_->SetNextScene(Scene::Model); }
	if (input->IsTrigger(DIK_3)) { sceneManager_->SetNextScene(Scene::Light); }
	if (input->IsTrigger(DIK_4)) { sceneManager_->SetNextScene(Scene::Collider); }

	ImGui::Text("1 Key : Sprite");
	ImGui::Text("2 Key : Model");
	ImGui::Text("3 Key : Light");
	ImGui::Text("4 Key : Collider");
	ImGui::Text("---------------------");
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