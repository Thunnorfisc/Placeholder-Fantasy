//============================================================================
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#include "pauseMenu.h"
#include <iostream>

PauseMenu::PauseMenu(SceneManager* manager)
{
    dxManager = manager;
    dxMenuText = new TextDX();     // DirectX fonts
}

PauseMenu::~PauseMenu()
{
    releaseAll();
    SAFE_DELETE(dxMenuText);
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void PauseMenu::initialize() 
{
    // Reset menu index to 0 on initialize 
    menuIndex = 0;
    // Cursor initialization
    if (!cursorTexture.initialize(dxManager->getGraphics(), Cursor))throw(gameErrorNS::FATAL_ERROR, "Error initiating Cursor");
    if (!cursor.initialize(dxManager->getGraphics(), 0, 0, 0, &cursorTexture))throw(gameErrorNS::FATAL_ERROR, "Error initiating Cursor");
    
    const float textSize = 25; // dxMenuText size
    const float originalCursorHeight = cursorTexture.getHeight(); // Original height of cursor
    const float originalCursorWidth = cursorTexture.getWidth();   // Original width of cursor
    const float scaledCursorHeight = textSize / originalCursorHeight;  // Height of cursor after scaling
    const float scaledCursorWidth = textSize / originalCursorWidth;    // Width of cursor after scaling

    // Set Background to white
    dxManager->getGraphics()->setBackColor(graphicsNS::WHITE);

    // initialize DirectX fonts
    // Options
    // Font: Trebuchet MS 
    if (dxMenuText->initialize(dxManager->getGraphics(), textSize, true, false, "Trebuchet MS") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    const std::vector<std::string> optionList = { "Back", "Stats", "Save", "Return to Title", "Exit Game" }; // Option list contains these options
    
    const int menuY = 100; // Start of the Y of menu options
    const int menuX = GAME_WIDTH / 15; // Split game_width to 15 parts, take the first part as X

    // Add to menuList the options from optionList
    for (int i = 0; i < optionList.size(); i++)
    {
        menuList.push_back({
            /*Option = */optionList.at(i),
            /*X = */menuX,
            /*Y = */menuY + 30 * i
            });
    }

    // Cursor settings on initialize
    cursor.setX(menuList.front().x - 30);
    cursor.setY(menuList.front().y);
    cursor.setScale(scaledCursorHeight, scaledCursorWidth);


    reset();            // reset all game variables
    return;
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void PauseMenu::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void PauseMenu::update(float frameTime)
{
    if (dxManager->getInput()->wasKeyPressed(VK_RETURN))
    {
        optionSelected(menuList.at(menuIndex).option);
    }
    if (dxManager->getInput()->wasKeyPressed(CURSOR_DOWN_KEY) && menuIndex != (menuList.size() - 1))               // if move down
    {
        menuIndex++;
        // cursor.setX(menuList.at(menuIndex).x - 20);
        cursor.setY(menuList.at(menuIndex).y);
    }
    if (dxManager->getInput()->wasKeyPressed(CURSOR_UP_KEY) && menuIndex != 0)               // if move up
    {
        menuIndex--;
        // cursor.setX(menuList.at(menuIndex).x - 20);
        cursor.setY(menuList.at(menuIndex).y);
    }
    cursor.update(frameTime);
}

//====================================================
// Check string of current option
// Return the function
// ===================================================
// *Switch case doesn't work because C++ doesn't allow strings in switch cases.
void PauseMenu::optionSelected(std::string option) {
    if (option == "Back")
    {
        dxManager->switchScene("Overworld");
    }
    else if (option == "Stats")
    {

    }
    else if (option == "Save")
    {
        std::ifstream file("placeholder_save.txt"); // Check if placeholder_save.txt is there
        if (!file.is_open()) // If not there
        {
            std::ofstream newFile("placeholder_save.txt"); // Create the txt file
            std::map<std::string, Var> map = dxManager->getState()->getMap(); // Get the map
            // Iterate through the map
            for (std::map<std::string, Var>::iterator it = map.begin();
                it != map.end();
                ++it)
            {
                // it->first is the keys, it->second is the Var
                newFile << it->first << " " << it->second.value << " " << it->second.type << std::endl;
            }
            newFile.close(); // Close the txt file
        }
        else
        {
            std::remove("placeholder_save.txt"); // Delete txt file
            std::ofstream newFile("placeholder_save.txt"); // Recreate txt file
            std::map<std::string, Var> map = dxManager->getState()->getMap(); // Get the map
            // Iterate through the map
            for (std::map<std::string, Var>::iterator it = map.begin();
                it != map.end();
                ++it)
            {
                // it->first is the keys, it->second is the Var
                newFile << it->first << " " << it->second.value << " " << it->second.type << std::endl;
            }
            newFile.close(); // Close the txt file
        }
    }
    else if (option == "Return to Title")
    {
        dxManager->getState()->resetState();
        dxManager->switchScene("Title");
    }
    else if (option == "Exit Game")
    {
        PostQuitMessage(0);
    }
}

void PauseMenu::ai()
{

}

void PauseMenu::collisions()
{

}

//=============================================================================
// render game items
//=============================================================================
void PauseMenu::render()
{
    dxManager->getGraphics()->spriteBegin();
    cursor.draw(TRANSCOLOR);
    dxMenuText->setFontColor(graphicsNS::BLACK);
    for (auto option : menuList)
    {
        dxMenuText->print(option.option, option.x, option.y);
    }
    dxManager->getGraphics()->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void PauseMenu::releaseAll()
{
    cursorTexture.onLostDevice();
    dxMenuText->onLostDevice();
    return;
}

//=============================================================================
// The graphics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void PauseMenu::resetAll()
{
    cursorTexture.onResetDevice();
    dxMenuText->onResetDevice();
    return;
}

// Neccessary Evil Why Please God No More LNK2001 Errors
void PauseMenu::onMessage(const Mail& mail)
{ }