#pragma once
#include "BaseScene.h"
#include "DebugCamera.h"
#include "Model.h"
#include "Objects.h"
#include <memory>

class AbstractScenes : public BaseScene
{
	bool isDebugMode = false;
protected:
	static ViewProjection viewProjection;
	static DebugCamera debugCamera;
	static LightGroup* lightGroup;
public:
	virtual void Initialize(){}
	static void StaticInitialize();
	void Update();
};

class SpriteScene : public AbstractScenes
{
	std::unique_ptr<Sprite> sprites[2];
public:
	void Initialize();
	void Update();
	void Draw();
};

class ModelScene : public AbstractScenes
{
	WorldTransform worldTransform;
	std::unique_ptr<Model> model;
public:
	void Initialize();
	void Update();
	void Draw();
};

class LightScene : public AbstractScenes
{
	Vector3 lightDir[3] =
	{
		{0,0,1},{0,1,0},{0,0,1}
	};
	Vector3 lightColor[3]
	{
		{1,0,0},{0,1,0},{0,0,1}
	};
public:
	void Initialize();
	void Update();
	void Draw();
};

class ColliderScene : public AbstractScenes
{
	std::vector<std::unique_ptr<Objects>> objects;
public:
	void Initialize();
	void Update();
	void Draw();
};