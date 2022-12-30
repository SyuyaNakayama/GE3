#include "SceneFactory.h"
#include "TitleScene.h"
#include "GamePlayScene.h"
#include "ResultScene.h"
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
    case Scene::Title:
        newScene = new TitleScene();
        break;
    case Scene::Play:
        newScene = new GamePlayScene();
        break;
    case Scene::GameOver:
    case Scene::Clear:
        newScene = new ResultScene(scene);
        break;
    }
    
    assert(newScene);
    return newScene;
}