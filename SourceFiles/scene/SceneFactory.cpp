#include "SceneFactory.h"
#include "AL4Scenes.h"
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
    case Scene::Sphere:     newScene = new SphereScene();   break;
    case Scene::Ray:        newScene = new RayScene();      break;
    case Scene::RayPlane:   newScene = new RayPlaneScene(); break;
    case Scene::RayCast:    newScene = new RayCastScene();  break;
    }
    
    assert(newScene);
    return newScene;
}