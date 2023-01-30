#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Objects.h"
#include "DebugCamera.h"

class AL4Scenes : public BaseScene
{
protected:
	ViewProjection viewProjection;
	DebugCamera debugCamera;
	std::vector<std::unique_ptr<Objects>> objects;
	LightGroup* lightGroup = nullptr;
	Vector3 lightDir[3] =
	{
		{0,0,1},{0,1,0},{0,0,1}
	};
	Vector3 lightColor[3]
	{
		{1,0,0},{0,1,0},{0,0,1}
	};

public:
	virtual ~AL4Scenes() = default;
	void Initialize();
	void Update();
	void Draw();
};

class SphereScene : public AL4Scenes
{
public:
	void Initialize();
};

class RayScene : public AL4Scenes
{
public:
	void Initialize();
};

class RayPlaneScene : public AL4Scenes
{
public:
	void Initialize();
};

class RayCastScene : public AL4Scenes
{
	bool isUseRayCast = true;
	Ray* ray = nullptr;
public:
	void Initialize();
	void Update();
};