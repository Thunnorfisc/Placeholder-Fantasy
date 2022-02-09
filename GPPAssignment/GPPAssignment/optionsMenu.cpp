//============================================================================
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#include "optionsMenu.h"
#include <iostream>
#include <nlohmann/json.hpp>

OptionsMenu::OptionsMenu(SceneManager* manager)
{
    dxManager = manager;
    dxMenuText = new TextDX();     // DirectX fonts
}

OptionsMenu::~OptionsMenu()
{
    dxManager->getAudio().stopSound(*dxOptionChange);
    releaseAll();
    SAFE_DELETE(dxMenuText);
    SAFE_DELETE(dxOptionChange);
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void OptionsMenu::initialize()
{
    // Reset menu index to 0 on initialize 
    menuIndex = 0;
    // Cursor initialization
    if (!cursorTexture.initialize(dxManager->getGraphics(), Cursor))throw(gameErrorNS::FATAL_ERROR, "Error initiating Cursor");
    if (!cursor.initialize(dxManager->getGraphics(), 0, 0, 0, &cursorTexture))throw(gameErrorNS::FATAL_ERROR, "Error initiating Cursor");

    const float textSize = (GAME_WIDTH * GAME_HEIGHT) / 36864; // dxMenuText size
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

    // Music Volume Text
    // Font: Trebuchet MS
    if (dxMusicVol.initialize(dxManager->getGraphics(), textSize, true, false, "Trebuchet MS") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // Sfx Volume Text
    // Font: Trebuchet MS
    if (dxSfxVol.initialize(dxManager->getGraphics(), textSize, true, false, "Trebuchet MS") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    const std::vector<std::string> optionList = { "Back", "Music", "Sound Effects" }; // Option list contains these options

    const int menuY = GAME_HEIGHT / 8; // Start of the Y of menu options
    const int menuX = GAME_WIDTH / 15; // Split game_width to 15 parts, take the first part as X

    // Add to menuList the options from optionList
    for (int i = 0; i < optionList.size(); i++)
    {
        menuList.push_back({
            /*Option = */optionList.at(i),
            /*X = */menuX,
            /*Y = */menuY + (GAME_HEIGHT / 24) * i
            });
    }

    // Cursor settings on initialize
    cursor.setX(menuList.front().x - GAME_WIDTH / 45);
    cursor.setY(menuList.front().y);
    cursor.setScale(scaledCursorHeight, scaledCursorWidth);

    dxOptionChange = new Sound(OPTIONCHANGESFX, audioTypes::Sfx);
    dxManager->getAudio().loadFile(*dxOptionChange);

    reset();            // reset all game variables
    return;
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void OptionsMenu::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void OptionsMenu::update(float frameTime)
{
    if (dxManager->getInput()->isKeyDown(VK_RETURN))
    {
        optionSelected(menuList.at(menuIndex).option);
    }
    if (dxManager->getInput()->wasKeyPressed(CURSOR_DOWN_KEY) && menuIndex != (menuList.size() - 1))               // if move down
    {
        dxManager->getAudio().playSound(*dxOptionChange);
        menuIndex++;
        // cursor.setX(menuList.at(menuIndex).x - 20);
        cursor.setY(menuList.at(menuIndex).y);
    }
    if (dxManager->getInput()->wasKeyPressed(CURSOR_UP_KEY) && menuIndex != 0)               // if move up
    {
        dxManager->getAudio().playSound(*dxOptionChange);
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
void OptionsMenu::optionSelected(std::string option) {
    if (option == "Back")
    {
        dxManager->getAudio().stopSound(*dxOptionChange);
        dxManager->getAudio().saveVolume();
        dxManager->unlayerScene();

    }
    else if (option == "Music")
    {
        // Volume going down
        // TESTING PURPOSES, REMOVE LATER...NOT <== NO LONGER FOR TESTING, COMMENT KEPT AS JOKE
        if (dxManager->getInput()->isKeyDown(CURSOR_LEFT_KEY))
        {
            if (dxManager->getAudio().getVolume(audioTypes::Music) >= 0.0f)
                dxManager->getAudio().setVolume(audioTypes::Music, dxManager->getAudio().getVolume(audioTypes::Music) - 0.01f);
        }
        // Volume going up
        // TESTING PURPOSES, REMOVE LATER...NOT <== NO LONGER FOR TESTING, COMMENT KEPT AS JOKE
        if (dxManager->getInput()->isKeyDown(CURSOR_RIGHT_KEY))
        {
            if (dxManager->getAudio().getVolume(audioTypes::Music) < 1.0f)
                dxManager->getAudio().setVolume(audioTypes::Music, dxManager->getAudio().getVolume(audioTypes::Music) + 0.01f);
        }
    }
    else if (option == "Sound Effects")
    {
        // Volume going down
        // TESTING PURPOSES, REMOVE LATER...NOT <== NO LONGER FOR TESTING, COMMENT KEPT AS JOKE
        if (dxManager->getInput()->isKeyDown(CURSOR_LEFT_KEY))
        {
            if (dxManager->getAudio().getVolume(audioTypes::Sfx) >= 0.0f)
            dxManager->getAudio().setVolume(audioTypes::Sfx, dxManager->getAudio().getVolume(audioTypes::Sfx) - 0.01f);
        }
        // Volume going up
        // TESTING PURPOSES, REMOVE LATER...NOT <== NO LONGER FOR TESTING, COMMENT KEPT AS JOKE
        if (dxManager->getInput()->isKeyDown(CURSOR_RIGHT_KEY))
        {
            if (dxManager->getAudio().getVolume(audioTypes::Sfx) < 1.0f)
                dxManager->getAudio().setVolume(audioTypes::Sfx, dxManager->getAudio().getVolume(audioTypes::Sfx) + 0.01f);
        }
    }
}

void OptionsMenu::ai()
{

}

void OptionsMenu::collisions()
{

}

//=============================================================================
// render game items
//=============================================================================
void OptionsMenu::render()
{
    std::string musicVol = to_string(int(dxManager->getAudio().getVolume(audioTypes::Music) * 100)) + "%";
    std::string sfxVol = to_string(int(dxManager->getAudio().getVolume(audioTypes::Sfx) * 100)) + "%";
    dxManager->getGraphics()->spriteBegin();
    cursor.draw(TRANSCOLOR);
    dxMenuText->setFontColor(graphicsNS::BLACK);
    dxMusicVol.setFontColor(graphicsNS::NAVY);
    dxSfxVol.setFontColor(graphicsNS::NAVY);
    for (auto option : menuList)
    {
        dxMenuText->print(option.option, option.x, option.y);
        if (option.option == "Music")
        {
            dxMusicVol.print(musicVol, option.x + dxMenuText->getWidth(option.option, dxMenuText->getFont()) + GAME_WIDTH / 45, option.y);
        }
        else if (option.option == "Sound Effects")
        {
            dxSfxVol.print(sfxVol, option.x + dxMenuText->getWidth(option.option, dxMenuText->getFont()) + GAME_WIDTH / 45, option.y);
        }
    }
    dxManager->getGraphics()->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void OptionsMenu::releaseAll()
{
    cursorTexture.onLostDevice();
    dxMenuText->onLostDevice();
    return;
}

//=============================================================================
// The graphics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void OptionsMenu::resetAll()
{
    cursorTexture.onResetDevice();
    dxMenuText->onResetDevice();
    return;
}

// Neccessary Evil Why Please God No More LNK2001 Errors
void OptionsMenu::onMessage(const Mail& mail)
{ }