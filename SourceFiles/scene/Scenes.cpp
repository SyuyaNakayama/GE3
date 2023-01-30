#include "Scenes.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include <imgui.h>
#include "ImGuiManager.h"

ViewProjection AbstractScenes::viewProjection;
DebugCamera AbstractScenes::debugCamera;
LightGroup* AbstractScenes::lightGroup;

void SpriteScene::Initialize()
{
	sprites[0] = Sprite::Create("flag1.png");
	sprites[1] = Sprite::Create("flag2.png");
	sprites[0]->SetSize({ 600, 400 });
	sprites[1]->SetSize({ 600, 400 });
	sprites[1]->SetPosition({ 640,0 });
	sprites[0]->SetPosition({ 300,360 });
	sprites[0]->SetAnchorPoint({ 0.5f,0.5f });
}

void SpriteScene::Update()
{
	AbstractScenes::Update();
	float rot = sprites[0]->GetRotation();
	rot += 0.025f;
	sprites[0]->SetRotation(rot);
	Vector2 pos = sprites[1]->GetPosition();
	pos.y += 2;
	sprites[1]->SetPosition(pos);
	for (const std::unique_ptr<Sprite>& sprite : sprites) { sprite->Update(); }
}

void SpriteScene::Draw()
{
	SpriteCommon::PreDraw();
	for (const std::unique_ptr<Sprite>& sprite : sprites) { sprite->Draw(); }
	SpriteCommon::PostDraw();
}

void ModelScene::Initialize()
{
	model = Model::Create("sphere");
	worldTransform.Initialize();
}

void ModelScene::Update()
{
	AbstractScenes::Update();
	worldTransform.Update();
}

void ModelScene::Draw()
{
	Model::PreDraw();
	model->Draw(worldTransform);
	Model::PostDraw();
}

void LightScene::Initialize()
{
}

void LightScene::Update()
{
	AbstractScenes::Update();
	ImGui::Text("---------------------");
	for (size_t i = 0; i < 3; i++)
	{
		ImGuiManager::SliderVector("lightDir" + std::to_string(i), lightDir[i], -10, 10);
		ImGuiManager::SliderVector("lightColor" + std::to_string(i), lightColor[i], 0, 1);
		lightGroup->SetDirLightDir(i, lightDir[i]);
		lightGroup->SetDirLightColor(i, lightColor[i]);
	}
	lightGroup->Update();
}

void LightScene::Draw()
{
}

void ColliderScene::Initialize()
{
	std::unique_ptr<Objects> obj = std::make_unique<Sphere>();
	obj->SetPosition({ 2 });
	objects.push_back(move(obj));
	obj = std::make_unique<Ray>();
	obj->SetMoveSpeed({ 0.05f });
	obj->SetPosition({ 0,2 });
	objects.push_back(move(obj));
	obj = std::make_unique<Triangle>();
	obj->SetPosition({ -2 });
	objects.push_back(move(obj));
	for (const std::unique_ptr<Objects>& object : objects) { object->Initialize(); }
	viewProjection.eye = { 0,2.5f,-5.0f, };
	viewProjection.target.y = 0;
}

void ColliderScene::Update()
{
	AbstractScenes::Update();
	for (const std::unique_ptr<Objects>& object : objects) { object->Update(); }
	CollisionManager::CheckAllCollisions();
}

void ColliderScene::Draw()
{
	Model::PreDraw();
	for (const std::unique_ptr<Objects>& object : objects) { object->Update(); }
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
}