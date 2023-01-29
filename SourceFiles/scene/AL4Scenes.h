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