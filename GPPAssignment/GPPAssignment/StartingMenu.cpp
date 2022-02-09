//============================================================================
//  Module:             Gameplay Programming
//  Assignment 2:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#include "startingMenu.h"
#include <iostream>

StartingMenu::StartingMenu(SceneManager* manager)
{
    dxManager = manager;
    dxMenuText = new TextDX();
    dxTitle = new TextDX();
    errorMsg = "";
}

StartingMenu::~StartingMenu()
{
    Mail mail(*this, dxManager->getAudio(), mailTypes::EndStream, dxMenuMusic);
    dxManager->getPostOffice()->addMessages(mail);
    dxManager->getAudio().stopSound(*dxOptionChange);
    //Mail mail2(*this, dxManager->getAudio(), mailTypes::StopSoundEvent, dxOptionChange);
    //dxManager->getPostOffice()->addMessages(mail2);
    releaseAll();
    SAFE_DELETE(dxMenuText);
    SAFE_DELETE(dxTitle);
    SAFE_DELETE(dxMenuMusic);
    SAFE_DELETE(dxOptionChange);
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void StartingMenu::initialize()
{
    // Reset menu index to 0 on initialize
    menuIndex = 0;

    // Initializing Cursor
    if (!cursorTexture.initialize(dxManager->getGraphics(), Cursor))throw(gameErrorNS::FATAL_ERROR, "Error initiating Cursor");
    if (!cursor.initialize(dxManager->getGraphics(), 0, 0, 0, &cursorTexture))throw(gameErrorNS::FATAL_ERROR, "Error initiating Cursor");

    // Set background colour: White
    dxManager->getGraphics()->setBackColor(graphicsNS::WHITE);

    const float textSize = (GAME_WIDTH*GAME_HEIGHT)/36864; // dxMenuText size
    const float titleSize = textSize * 4; // dxTitle size
    const float originalCursorHeight = cursorTexture.getHeight(); // Original height of cursor
    const float originalCursorWidth = cursorTexture.getWidth();   // Original width of cursor
    const float scaledCursorHeight = textSize / originalCursorHeight;  // Height of cursor after scaling
    const float scaledCursorWidth = textSize / originalCursorWidth;    // Width of cursor after scaling

    // initialize DirectX fonts
    // Option Text
    // Font: Trebuchet MS
    if (dxMenuText->initialize(dxManager->getGraphics(), textSize, true, false, "Trebuchet MS") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // Title
    // Font: Old English Text MT
    if (dxTitle->initialize(dxManager->getGraphics(), titleSize, true, false, "Old English Text MT") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // Error Text
    // Font: Trebuchet MS
    if (dxError.initialize(dxManager->getGraphics(), textSize, true, false, "Trebuchet MS") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    std::vector<std::string> optionList = { "Start New Adventure", "Continue", "Options", "Quit" }; // Option list contains these options

    const int menuY = GAME_HEIGHT / 2; // Half of game height

    // Add to menuList the options from optionList
    for (int i = 0; i < optionList.size(); i++)
    {
        menuList.push_back({ 
            /*Option = */optionList.at(i),
            /*X = */static_cast<int>(GAME_WIDTH / 2 - dxMenuText->getWidth(optionList.at(i), dxMenuText->getFont()) / 2),
            /*Y = */menuY + (GAME_HEIGHT/24) * i
            });
    }

    // Cursor settings on initialize
    cursor.setX(menuList.front().x - GAME_WIDTH/45);
    cursor.setY(menuList.front().y);
    cursor.setScale(scaledCursorHeight, scaledCursorWidth);

    // Sound/Music Initialize
    // Menu Music
    dxMenuMusic = new Stream(MENUMUSIC, true, audioTypes::Music);
    dxOptionChange = new Sound(OPTIONCHANGESFX, audioTypes::Sfx);
    dxManager->getAudio().loadFile(*dxOptionChange);
    // Start Menu Music
    Mail mail(*this, dxManager->getAudio(), mailTypes::BeginStream, dxMenuMusic);
    dxManager->getPostOffice()->addMessages(mail);
    musicIsPlaying = false;

    reset();            // reset all game variables
    return;
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void StartingMenu::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void StartingMenu::update(float frameTime)
{
    // When enter is pressed, check for which string it's currently at
    if (dxManager->getInput()->wasKeyPressed(VK_RETURN)) 
    {
        optionSelected(menuList.at(menuIndex).option);
    }
    // Move down
    if (dxManager->getInput()->wasKeyPressed(CURSOR_DOWN_KEY) && menuIndex != (menuList.size() - 1))
    {
        dxManager->getAudio().playSound(*dxOptionChange);
        menuIndex++; // Increases menu index to tell where the location of the cursor is
        // cursor.setX(menuList.at(menuIndex).x - 20);
        cursor.setY(menuList.at(menuIndex).y);
    }
    // Move up
    if (dxManager->getInput()->wasKeyPressed(CURSOR_UP_KEY) && menuIndex != 0)
    {
        dxManager->getAudio().playSound(*dxOptionChange);
        menuIndex--; // Decreases menu index to tell where the location of the cursor is
        // cursor.setX(menuList.at(menuIndex).x - 20);
        cursor.setY(menuList.at(menuIndex).y);
    }
    cursor.update(frameTime);
}


//====================================================
// Check string of current option
// Return the function
//====================================================
// *Switch case doesn't work because C++ doesn't allow strings in switch cases.
void StartingMenu::optionSelected(std::string option) {
    // Start New Adventure -> Overworld at default position
    if (option == "Start New Adventure")
    {
        Mail streamMail(*this, dxManager->getAudio(), mailTypes::EndStream, dxMenuMusic);
        dxManager->getPostOffice()->addMessages(streamMail);
        dxManager->getAudio().stopSound(*dxOptionChange);
        dxManager->getState()->resetState();
        dxManager->switchScene("StartingRoom");
    }
    // Continue -> Overworld at saved position
    else if (option == "Continue")
    {
        Mail mail(*this, dxManager->getAudio(), mailTypes::EndStream, dxMenuMusic);
        dxManager->getPostOffice()->addMessages(mail);
        dxManager->getAudio().stopSound(*dxOptionChange);
        // Read file PH_save.json
        std::ifstream file("PH_save.json");
        if (file.is_open())
        {
            nlohmann::json savejson;
            file >> savejson;
            std::string scene = savejson["scene"];
            dxManager->switchScene(scene);
            file.close();
        }
        else
        {
            throw(GameError(gameErrorNS::FATAL_ERROR, "Save file not detected!"));
        }
    }
    // Option -> Whatever options we have
    else if (option == "Options")
    {
        dxManager->layerScene("Options");
    }
    // Quit -> Quit the game
    else if (option == "Quit")
    {
        Mail mail(*this, dxManager->getAudio(), mailTypes::EndStream, dxMenuMusic);
        dxManager->getPostOffice()->addMessages(mail);
        dxManager->getAudio().stopSound(*dxOptionChange);
        PostQuitMessage(0);
    }
}

void StartingMenu::ai()
{
}

void StartingMenu::collisions()
{
}

//=============================================================================
// render game items
//=============================================================================
void StartingMenu::render()
{
    std::string musicVol = to_string(int(dxManager->getAudio().getVolume(audioTypes::Music) * 100)) + "%";
    dxManager->getGraphics()->spriteBegin();
    cursor.draw(TRANSCOLOR);
    dxTitle->setFontColor(graphicsNS::ORANGE);
    dxTitle->print("PlaceHolder Fantasy?", static_cast<int>(GAME_WIDTH / 2 - dxTitle->getWidth("PlaceHolder Fantasy?", dxTitle->getFont()) / 2), GAME_HEIGHT / 2 - GAME_HEIGHT / 5);
    // dxError.print("(Left Arrow)<- Volume of Music: " + musicVol + " ->(Right Arrow)", static_cast<int>(GAME_WIDTH / 2 - dxError.getWidth("Volume of Music: ", dxError.getFont()) / 2), GAME_HEIGHT / 2 - 200);
    // dxError.print(errorMsg, static_cast<int>(GAME_WIDTH / 2 - dxTitle->getWidth("PlaceHolder Fantasy?", dxTitle->getFont()) / 2), GAME_HEIGHT / 2 - 200);
    dxMenuText->setFontColor(graphicsNS::BLACK);
    for(auto option: menuList) 
    {
        dxMenuText->print(option.option, option.x, option.y);
    }
    dxManager->getGraphics()->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void StartingMenu::releaseAll()
{
    cursorTexture.onLostDevice();
    dxMenuText->onLostDevice();
    dxTitle->onLostDevice();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void StartingMenu::resetAll()
{
    cursorTexture.onResetDevice();
    dxMenuText->onResetDevice();
    dxTitle->onResetDevice();
    return;
}

// Neccessary Evil Why Please God No More LNK2001 Errors
void StartingMenu::onMessage(const Mail& mail)
{ }