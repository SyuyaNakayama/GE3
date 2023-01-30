#include "AL4Scenes.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Quaternion.h"
#include <imgui.h>
#include "ImGuiManager.h"

void AL4Scenes::Initialize()
{
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	for (const std::unique_ptr<Objects>& object : objects) { object->Initialize(); }
}

void AL4Scenes::Update()
{
	if (input->IsTrigger(DIK_1)) { sceneManager_->SetNextScene(Scene::Sphere); }
	if (input->IsTrigger(DIK_2)) { sceneManager_->SetNextScene(Scene::Ray); }
	if (input->IsTrigger(DIK_3)) { sceneManager_->SetNextScene(Scene::RayPlane); }
	if (input->IsTrigger(DIK_4)) { sceneManager_->SetNextScene(Scene::RayCast); }

	for (const std::unique_ptr<Objects>& object : objects) { object->Update(); }

	CollisionManager::CheckAllCollisions();

	debugCamera.Update();

	WorldTransform w; w.Initialize();
	viewProjection.Update();
	for (size_t i = 0; i < 1000; i++)
	{
		w.Update();
	}
}

void AL4Scenes::Draw()
{
	Model::PreDraw();
	for (const std::unique_ptr<Objects>& object : objects) { object->Draw(); }
	Model::PostDraw();
}

void SphereScene::Initialize()
{
	std::unique_ptr<Objects> obj = std::make_unique<Sphere>();
	obj->SetPosition({ 0,2 });
	obj->SetMoveSpeed({ 0,-0.08f });
	objects.push_back(move(obj));
	obj = std::make_unique<Plane>();
	objects.push_back(move(obj));
	obj = std::make_unique<Triangle>();
	obj->SetPosition({ 0,3 });
	objects.push_back(move(obj));
	AL4Scenes::Initialize();
	viewProjection.eye = { 0,5.0f,-10.0f, };
	viewProjection.target.y = 0.0f;
}

void RayScene::Initialize()
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
	AL4Scenes::Initialize();
	viewProjection.eye = { 0,2.5f,-5.0f, };
	viewProjection.target.y = 0;
}

void RayPlaneScene::Initialize()
{
	std::unique_ptr<Objects> obj = std::make_unique<Plane>();
	objects.push_back(move(obj));
	obj = std::make_unique<Ray>();
	obj->SetMoveSpeed({ 0,0.08f });
	obj->SetPosition({ 0,2 });
	objects.push_back(move(obj));
	AL4Scenes::Initialize();
	viewProjection.eye = { 0,2.5f,-15.0f, };
	viewProjection.target.y = 0;
}

void RayCastScene::Initialize()
{
	std::unique_ptr<Objects> obj = std::make_unique<Sphere>();
	objects.push_back(move(obj));
	obj = std::make_unique<Plane>();
	obj->SetPosition({ -2 });
	objects.push_back(move(obj));
	ray = new Ray;
	obj.reset(ray);
	obj->SetMoveSpeed({ 0.08f });
	obj->SetPosition({ 0,2 });
	objects.push_back(move(obj));
	AL4Scenes::Initialize();
	viewProjection.eye = { 0,2.5f,-15.0f, };
	viewProjection.target.y = 0;

}

void RayCastScene::Update()
{
	if (input->IsTrigger(DIK_1)) { sceneManager_->SetNextScene(Scene::Sphere); }
	if (input->IsTrigger(DIK_2)) { sceneManager_->SetNextScene(Scene::Ray); }
	if (input->IsTrigger(DIK_3)) { sceneManager_->SetNextScene(Scene::RayPlane); }
	if (input->IsTrigger(DIK_4)) { sceneManager_->SetNextScene(Scene::RayCast); }

	ImGui::Text("1 Key : SphereCollision");
	ImGui::Text("2 Key : RayCollision");
	ImGui::Text("3 Key : Ray&PlaneCollision");
	ImGui::Text("4 Key : RayCastCollision");
	ImGui::Text("Q Key : CollisionModeChange");
	if (!isUseRayCast) { ImGui::Text("Mode  : CollisionAll"); }
	else { ImGui::Text("Mode  : CollisionNearly"); }

	for (const std::unique_ptr<Objects>& object : objects) { object->Update(); }
	if (input->IsTrigger(DIK_Q)) { isUseRayCast = !isUseRayCast; }
	if (isUseRayCast) { CollisionManager::CheckRayCastCollision(ray); }
	else { CollisionManager::CheckAllCollisions(); }
	viewProjection.Update();
}
