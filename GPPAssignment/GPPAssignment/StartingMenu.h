//============================================================================<<
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================<>
// Description of Starting Menu:
// This is the starting menu, the title screen of the game. 
// This is the first thing
//      loaded into the game.
// Pressing continue will load your coordinates from the last time you saved.
// Starting a new adventure would load you to the default of the coordinates.
// Quit...quits the game

#ifndef _STARTINGMENU_H             // prevent multiple definitions if this 
#define _STARTINGMENU_H             // ..file is included in more than one place
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
#include "gameState.h"
#include "audio.h"
#include <vector>
#include <fstream>

//===================
// MENU CLASS
//===================
class StartingMenu: public Scene
{
protected:
    SceneManager* dxManager;          // Call to the SceneManager
    TextDX  dxError;                  // Error Text
    TextDX* dxTitle;                  // Title Text
    TextDX* dxMenuText;               // Menu Text
    Stream* dxMenuMusic;               // Menu Music
    Sound* dxOptionChange;            // Change Options Sound Effect
    TextureManager cursorTexture;     // Cursor Texture

    std::string errorMsg;             // Error Message
    bool musicIsPlaying;              // Check if music is playing

    int menuIndex = 0;                // Indexing for menu
    std::vector<MenuOption> menuList; // The list of menuOptions to allow menu creation

    Image cursor;                     // Cursor Object
public:
    // Constructor
    StartingMenu(SceneManager* manager);

    // Destructor
    virtual ~StartingMenu();

    void initialize();
    void reset();
    void update(float frameTime);
    void optionSelected(std::string option);  // For menu selection
    void ai();
    void collisions();
    void render();
    void releaseAll();
    void resetAll();
    void onMessage(const Mail& mail);
};
#endif
