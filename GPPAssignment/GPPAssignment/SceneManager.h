//============================================================================<<
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================<>
// Description of SceneManager:
// The overlord of the scene system. 
// This is the game itself. It is a child of the Game class and is responsible for initializing the
//		application. It also manages the scene and allows scene switching to occur. 
// All scenes will take after the methods of the sceneManager and
//		adapt it for their own logic. The SceneManager is in charge of running the scenes, behind-the-scenes.
//============================================================================>>
#ifndef _SCENEMANAGER_H             // prevent multiple definitions if this 
#define _SCENEMANAGER_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <map>
#include <vector>
#include <stack>
#include "stack.hpp"
#include "game.h"
#include "scene.h"
#include "gameState.h"
#include "textDX.h"

class SceneManager: public Game
{
protected:
	TextDX dxFPS;
	Stack<Scene*> sceneStack;                   // Stack of scenes
	GameState currentState;						// The state the game has, keeps data that is a global variable
	std::vector<Character> characterList;		// The list of characters
	std::map<std::string, Scene*> sceneMap;     // Map of scenes, used to switch scenes and keep scenes
	 
public:
	SceneManager();
	~SceneManager();

	// Switching Scene
	void switchScene(std::string scene);

	// When the character list is needed when switching scene
	void switchScene(std::string scene, std::vector<Character> characterList);

	// Layer scene over other scenes
	void layerScene(std::string scene);

	// Layer scene over other scenes with character list
	void layerScene(std::string scene, std::vector<Character> characterList);

	// Unlayer scene for previous scene
	void unlayerScene();

	void initialize(HWND hwnd);

	// These methods are inherited from Game. Used to call the respective methods in the scenes.
	void reset();
	void update();
	void ai();
	void collisions();
	void render();
	void releaseAll();
	void resetAll();

	std::vector<Character>* getCharacterList() { return sceneStack.getTop()->getCharacterList(); }
	// Gets the game state
	GameState* getState() { return &currentState;  }
};
#endif
