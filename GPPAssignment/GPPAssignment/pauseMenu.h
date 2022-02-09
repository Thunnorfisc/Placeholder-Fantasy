//============================================================================<<
//  Module:             Gameplay Programming
//  Assignment 2:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================<>
// Description of PauseMenu:
// This is the pause menu. Only accessible in the overworld. 
// It can save, return to title and exit the game.
// When this is called, the game is "Paused", 
//      the characters coordinates are put into the global map for when they want to return to the game.
// If they return to the title or exit the game, gameState is wiped! (AKA Global Map is cleared).
// When they save, world coordinates are placed on a text file for future loads.
//============================================================================>>
#ifndef _PAUSEMENU_H             // prevent multiple definitions if this 
#define _PAUSEMENU_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textDX.h"
#include "constants.h"
#include "TextureManager.h"
#include "Image.h"
#include "graphics.h"
#include "Scene.h"
#include "sceneManager.h"
#include "menuOption.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

//===================
// MENU CLASS
//===================
class PauseMenu : public Scene
{
protected:
    SceneManager* dxManager;          // Call to the SceneManager
    TextDX  dxError;                  // Error Text
    TextDX* dxMenuText;               // Menu Text
    Sound* dxOptionChange;            // Change Options Sound Effect
    TextureManager cursorTexture;     // Cursor Texture

    int menuIndex = 0;                // Indexing for menu
    std::vector<MenuOption> menuList; // The list of menuOptions to allow menu creation

    Image cursor;                     // Cursor Object
public:
    // Constructor
    PauseMenu(SceneManager* manager);

    // Destructor
    virtual ~PauseMenu();

    void initialize();
    void reset();
    void update(float frameTime);
    void optionSelected(std::string option);   // For menu selection
    void ai();
    void collisions();
    void render();
    void releaseAll();
    void resetAll();
    void onMessage(const Mail& mail);
};
#endif
