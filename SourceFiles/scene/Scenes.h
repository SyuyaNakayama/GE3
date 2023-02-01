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
	WorldTransform worldTransformSmooth;
	WorldTransform skydome;
	WorldTransform ground;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> modelSmooth;
	std::unique_ptr<Model> modelSkydome;
	std::unique_ptr<Model> modelGround;

	bool dirLightActive[3] = { true,true,true };
	Vector3 dirLightDir[3] = { {1,0,0},{0,1,0},{0,0,1} };
	ColorRGB dirLightColor[3] = { {1,0,0},{0,1,0},{0,0,1} };
	bool pointLightActive[3] = { true,false,false };
	Vector3 pointLightpos[3] = { {0,1.0f} };
	ColorRGB pointLightColor[3];
	Vector3 pointLightAtten[3] = { {0.3f,0.1f,0.1f} };
public:
	void Initialize();
	void Update();
	void Draw();
};