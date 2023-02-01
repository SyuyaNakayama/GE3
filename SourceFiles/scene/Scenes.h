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
	virtual void Initialize() {}
	static void StaticInitialize();
	void Update();
};

class LightScene : public AbstractScenes
{
	WorldTransform worldTransform;
	WorldTransform worldTransformCube;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> modelSmooth;

	Vector3 lightDir[3] =
	{
		{0,0,1},{0,1,0},{0,0,1}
	};
	ColorRGB lightColor[3]
	{
		{1,0,0},{0,1,0},{0,0,1}
	};
	Vector3 pointLightpos = { 0.5f,1.0f };
	ColorRGB pointLightColor;
	Vector3 pointLightAtten = { 0.3f,0.1f,0.1f };
public:
	void Initialize();
	void Update();
	void Draw();
};