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
        newScene = new SpriteScene;
        break;
    case Scene::Model:
        newScene = new ModelScene;
        break;
    case Scene::Light:
        newScene = new LightScene;
        break;
    case Scene::Collider:
        newScene = new ColliderScene;
        break;
    }
    
    assert(newScene);
    return newScene;
}