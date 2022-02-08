//============================================================================
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#include "SceneManager.h"
#include "StartingMenu.h"
#include "battleScene.h"
#include "StartingRoom.h"
#include "pauseMenu.h"

SceneManager::SceneManager()
{
    // Map of scenes here, use dxManager->switchScene(the one you want to switch to) to switch scenes
    // Format: sceneMap["Key"] = new SceneName(this);
    sceneMap["Title"] = new StartingMenu(this);
    sceneMap["StartingWorld"] = new StartingRoom(this);
    //sceneMap["Overworld"] = new StartingRoom(this);
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
    // Clear the stack to prevent double same scene initialized in the stack
    sceneStack.clearStack();
    // Push the new scene to the stack
    sceneStack.push(sceneMap[scene]);
    // Initialize the scene
    sceneStack.getTop()->initialize();
}

void SceneManager::switchScene(std::string scene, std::vector<Character> characterList)
{
    // Clear the stack to prevent double same scene initialized in the stack
    sceneStack.clearStack();
    // Push the new scene to the stack
    sceneStack.push(sceneMap[scene]);
    // Set the character list
    sceneStack.getTop()->setCharacterList(characterList);
    // Initialize the scene
    sceneStack.getTop()->initialize();
}

// dxManager->layerScene(the one you want to switch to) to layer scenes
void SceneManager::layerScene(std::string scene)
{
    // Push the new scene on top of the stack
    sceneStack.push(sceneMap[scene]);
    // Initialize the scene
    sceneStack.getTop()->initialize();
}

void SceneManager::layerScene(std::string scene, std::vector<Character> characterList)
{
    // Push the new scene on top of the stack
    sceneStack.push(sceneMap[scene]);
    // Set the character list
    sceneStack.getTop()->setCharacterList(characterList);
    // Initialize the scene
    sceneStack.getTop()->initialize();
}

// To go back to the previous scene (AKA the second scene in the stack), call dxManager->unlayerScene()
void SceneManager::unlayerScene()
{
    // Remove the top scene to go back to the previous scene
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