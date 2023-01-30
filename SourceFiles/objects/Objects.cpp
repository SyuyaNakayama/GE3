#include "Objects.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <random>

void Objects::ChangeColor(Color color)
{
	Sprite* textureSprite = model->GetSprite();
	textureSprite->SetColor(color);
	model->TextureUpdate();
}

void Sphere::Initialize()
{
	model = Model::Create("sphere", true);
	worldTransform.Initialize();
}

void Sphere::Update()
{
	ChangeColor({ 1,1,1,1 });
	if (Length(Vector3(0, 2.0f) - worldTransform.translation) >= 3.0f) { moveSpd = -moveSpd; }
	worldTransform.translation += moveSpd;
	worldTransform.rotation.y += 0.05f;
	worldTransform.Update();
}

void Plane::Initialize()
{
	model = Model::Create("cube");
	worldTransform.Initialize();
	worldTransform.scale = { 5.0f,0.01f,5.0f };
}

void Plane::Update()
{
	ChangeColor({ 1,1,1,1 });
	worldTransform.translation = GetNormal() * distance;
	worldTransform.Update();
}

void Ray::Initialize()
{
	model = Model::Create("cube");
	worldTransform.scale *= 0.5f;
	worldTransform.rotation.x = PI / 2.0f;
	worldTransform.Initialize();
	child.scale = { 0.2f,0.2f,200.0f };
	child.translation.z = child.scale.z;
	child.parent = &worldTransform;
	child.Initialize();
	ChangeColor({ 0,0,1,1 });
}

void Ray::Update()
{
	worldTransform.translation += moveSpd;
	if (worldTransform.translation.Length() >= 4.0f) { moveSpd = -moveSpd; }
	worldTransform.Update();
	child.Update();
}

void Ray::Draw()
{
	model->Draw(worldTransform);
	model->Draw(child);
}

void Triangle::Initialize()
{
	model = Model::Create("triangle");
	worldTransform.Initialize();
	vertices.push_back({ -0.5f,-0.5f,0 });
	vertices.push_back({ 0,0.5f,0 });
	vertices.push_back({ 0.5f,-0.5f,0 });
	baseNormal = -Vector3::MakeZAxis();
	worldTransform.scale *= 2.0f;
	worldTransform.rotation.x = PI / 2.0f;
	worldTransform.Update();
	UpdateVertices();
}

void Triangle::Update()
{
	ChangeColor({ 1,1,1,1 });
	worldTransform.Update();
}
