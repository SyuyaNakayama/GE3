#pragma once
#include "BaseScene.h"
#include <string>

enum class Scene { Null, Title, Play };

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;
	virtual BaseScene* CreateScene(Scene scene) = 0;
};