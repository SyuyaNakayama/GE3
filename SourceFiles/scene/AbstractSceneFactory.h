#pragma once
#include "BaseScene.h"
#include <string>

enum class Scene { Null, Sprite, Model, Light, Collider };

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;
	virtual BaseScene* CreateScene(Scene scene) = 0;
};