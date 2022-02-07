//============================================================================
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#include "SceneManager.h"
#include "StartingMenu.h"
#include "battleScene.h"
#include "overworld.h"
#include  "pauseMenu.h"

SceneManager::SceneManager()
{
    // Map of scenes here, use dxManager->switchScene(the one you want to switch to) to switch scenes
    // Format: sceneMap["Key"] = new SceneName(this);
    sceneMap["Title"] = new StartingMenu(this);
    sceneMap["Overworld"] = new Overworld(this);
    //sceneMap["Battle"] = new BattleScene(this);
    sceneMap["PauseMenu"] = new PauseMenu(this);
}


SceneManager::~SceneManager()
{
	SceneManager::releaseAll();
    sceneStack.~Stack();
    // Delete all pointers
    std::map<std::string, Scene*>::iterator i;
    for (i = sceneMap.begin(); i != sceneMap.end(); i++)
    {
        SAFE_DELETE(sceneMap[i->first]);
    }
}
// dxManager->switchScene(the one you want to switch to) to switch scenes
void SceneManager::switchScene(std::string scene)
{
    sceneStack.clearStack();
    sceneStack.push(sceneMap[scene]);
    sceneStack.getTop()->initialize();
}

void SceneManager::switchScene(std::string scene, std::vector<Character> characterList)
{
    sceneStack.clearStack();
    sceneStack.push(sceneMap[scene]);
    sceneStack.getTop()->setCharacterList(characterList);
    sceneStack.getTop()->initialize();
}

void SceneManager::layerScene(std::string scene)
{
    sceneStack.push(sceneMap[scene]);
    sceneStack.getTop()->initialize();
}

void SceneManager::layerScene(std::string scene, std::vector<Character> characterList)
{
    sceneStack.push(sceneMap[scene]);
    sceneStack.getTop()->setCharacterList(characterList);
    sceneStack.getTop()->initialize();
}

void SceneManager::unlayerScene()
{
    sceneStack.pop();
}

// Initialize SceneManager with title/main menu
void SceneManager::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    sceneStack.push(sceneMap["Title"]);
    sceneStack.getTop()->initialize();
}

void SceneManager::reset()
{
    sceneStack.getTop()->reset();
    return;
}

void SceneManager::update()
{
    sceneStack.getTop()->update(frameTime);
}

void SceneManager::ai()
{
    sceneStack.getTop()->ai();
}

void SceneManager::collisions()
{
    sceneStack.getTop()->collisions();
}

//=============================================================================
// render game items
//=============================================================================
void SceneManager::render()
{
    sceneStack.getTop()->render();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void SceneManager::releaseAll()
{
    sceneStack.getTop()->releaseAll();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void SceneManager::resetAll()
{
    sceneStack.getTop()->resetAll();
    Game::resetAll();
    return;
}