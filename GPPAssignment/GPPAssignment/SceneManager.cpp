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
}
// dxManager->switchScene(the one you want to switch to) to switch scenes
void SceneManager::switchScene(std::string scene)
{
    currentScene = sceneMap[scene];
    currentScene->initialize();
}

void SceneManager::switchScene(std::string scene, std::vector<Character> characterList)
{
    currentScene = sceneMap[scene];
    currentScene->setCharacterList(characterList);
    currentScene->initialize();
}

// Initialize SceneManager with title/main menu
void SceneManager::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    currentScene = sceneMap["Title"];
    currentScene->initialize();
}

void SceneManager::reset()
{
    currentScene->reset();
    return;
}

void SceneManager::update()
{
    if (currentScene->getPause() == false)
        currentScene->update(frameTime);
}

void SceneManager::ai()
{
    if (currentScene->getPause() == false)
        currentScene->ai();
}

void SceneManager::collisions()
{
    if (currentScene->getPause() == false)
        currentScene->collisions();
}

//=============================================================================
// render game items
//=============================================================================
void SceneManager::render()
{
    if (currentScene->getPause() == false)
        currentScene->render();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void SceneManager::releaseAll()
{
    currentScene->releaseAll();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void SceneManager::resetAll()
{
    currentScene->resetAll();
    Game::resetAll();
    return;
}