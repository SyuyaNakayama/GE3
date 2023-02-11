#pragma once
#include "BaseScene.h"
#include "DebugCamera.h"
#include "Model.h"
#include "Objects.h"
#include "Skydome.h"

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
	WorldTransform worldTransformSmooth;
	WorldTransform ground;
	std::unique_ptr<Model> modelSmooth;
	std::unique_ptr<Model> modelGround;
	Skydome skydome;

	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 0.5f,0.6f };
	Vector2 circleShadowFactorAngle = { 0,0.5f };

public:
	void Initialize();
	void Update();
	void Draw();
};