#include "SceneFactory.h"
#include "Scenes.h"
#include <cassert>

SceneFactory* SceneFactory::GetInstance()
{
    static SceneFactory instance;
    return &instance;
}

BaseScene* SceneFactory::CreateScene(Scene scene)
{
    BaseScene* newScene = nullptr;

    switch (scene)
    {
    case Scene::Sprite:
        break;
    case Scene::Model:
        break;
    case Scene::Light:
        newScene = new LightScene;
        break;
    case Scene::Collider:
        break;
    }
    
    assert(newScene);
    return newScene;
}