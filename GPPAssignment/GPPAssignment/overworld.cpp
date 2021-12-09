#include "overworld.h"
#include <iostream>
using namespace std;

Overworld::Overworld(SceneManager* manager) {
    dxManager = manager;
}

Overworld::~Overworld()
{
    releaseAll();
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void Overworld::initialize()
{
    characterList.clear();

    //Initialize the world map
    initializeWorld();

    //initialize the Characters
    initializeCharacters();

    return;
}

void Overworld::reset()
{
    return;
}

//=============================================================================
// Update to check for game logic
// frameTime is used to regulate the speed of movement
//=============================================================================
void Overworld::update(float frameTime)
{
    //Checking for controls
    controls(frameTime);

    // Goes to pause menu
    if (dxManager->getInput()->wasKeyPressed(VK_ESCAPE)) 
    {
        // Store world coordinates into game state
        dxManager->getState()->setValueToState("WorldX", worldX);
        dxManager->getState()->setValueToState("WorldY", worldY);
        dxManager->switchScene("PauseMenu");
    }
    //UpdatePositions
    setWorldPosition();
}

void Overworld::ai()
{

}

void Overworld::collisions()
{
    VECTOR2 collisionVector;
    bool check = (playerChara.collidesWith(enemyChara, collisionVector));
    if (playerChara.collidesWith(enemyChara, collisionVector))
    {
        characterList.push_back(enemyChara);
        enemyChara.setActive(false);
        playerChara.setActive(false);
        dxManager->switchScene("Battle", characterList);
    }
}

//=============================================================================
//                          Render game items
//=============================================================================
void Overworld::render()
{
    dxManager->getGraphics()->spriteBegin();

    //Draw the world map
    worldMap.draw(TRANSCOLOR);

    //Draw enemy Character
    enemyChara.draw(graphicsNS::RED);
    enemyChara.setActive(true);

    //Draw Main Character
    playerChara.draw();
    playerChara.setActive(true);

    //renderCharacters();
    dxManager->getGraphics()->spriteEnd();
}

//=============================================================================
// The dxManager->getGraphics() device was lost.
// Release all reserved video memory so dxManager->getGraphics() device may be reset.
//=============================================================================
void Overworld::releaseAll()
{
    worldMapTexture.onLostDevice();
    mainCharaTexture.onLostDevice();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void Overworld::resetAll()
{
    worldMapTexture.onResetDevice();
    mainCharaTexture.onResetDevice();
    return;
}

//Initialize the world and set default position
void Overworld::initializeWorld()
{
    //initializing World map Texture
    if (!worldMapTexture.initialize(dxManager->getGraphics(), OVERWORLD_IMAGE))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize World Map initialize"));

    //initializing World Map
    if (!worldMap.initialize(dxManager->getGraphics(), 0, 0, 0, &worldMapTexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize World Map"));

    //Scaling Up world
    worldMap.setScale(3, 3);

    //If map stuff doesn't exist, tell it to go to default
    if ((dxManager->getState()->isExistInState("WorldX") == false) && (dxManager->getState()->isExistInState("WorldY") == false))
    {
        dxManager->getState()->setValueToState("WorldX", -worldMap.getWidth() / 2 + GAME_WIDTH / 2);
        dxManager->getState()->setValueToState("WorldY", -worldMap.getHeight() / 2 + GAME_HEIGHT / 2);
    }

    //Setting Default Position of the world
    worldMap.setX(dxManager->getState()->getFloatFromState<float>("WorldX"));
    worldMap.setY(dxManager->getState()->getFloatFromState<float>("WorldY"));

    //Setting world parameters
    worldX = worldMap.getX();
    worldY = worldMap.getY();
}


//=====================================================================================
//              Set Main Character to always be center of the screen
//            Rest of the Characters are set base on WorldX and WorldY
// 
//            E.g. test.setX(WorldX + X), X being the world cooridinate
//=====================================================================================
void Overworld::initializeCharacters() 
{
    //initializing Character Texture
    if (!mainCharaTexture.initialize(dxManager->getGraphics(), MAINCHARA_IMAGE))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Main Character texture"));

    //initializing player character
    if (!playerChara.initialize(dxManager, 0, 0, 0, &mainCharaTexture, "Ayame","Hero",100,true))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Main Character"));

    //Setting Scale
    playerChara.setScale(1.5, 1.5);

    //Setting Default Position of the world
    playerChara.setX(GAME_WIDTH / 2 - playerChara.getImagePtr()->getWidth() / 2);
    playerChara.setY(GAME_HEIGHT / 2 - playerChara.getImagePtr()->getHeight() / 2);

    characterList.push_back(playerChara);

    //initializing enemyCharacter
    if (!enemyChara.initialize(dxManager, 0, 0, 0, &mainCharaTexture, "WoD", "Tank", 100))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize enemyCharacter"));

    //Getting the right orientationa and scale
    enemyChara.setScale(1.5, 1.5);
    enemyChara.getImagePtr()->flipHorizontal(true);

    //Setting Enemy position
    enemyChara.setX(worldX + GAME_WIDTH / 2);
    enemyChara.setY(worldY + 1840);

}



//=====================================================================================
//                                  Movement Controls
//                              Updates WorldX and WorldY
//=====================================================================================
void Overworld::controls(float frameTime) {
    bool bruh = playerChara.getX() + playerChara.getWidth() <= GAME_WIDTH;
    float check = playerChara.getX() + playerChara.getWidth();
    float check2 = GAME_WIDTH;
    // Right Key
    if (dxManager->getInput()->isKeyDown(NAVI_RIGHT_KEY))
    {
        if (playerChara.getX() >= GAME_WIDTH / 2 - playerChara.getWidth() / 2 && -worldX + GAME_WIDTH <= (worldMap.getWidth()))
        {
            worldX -= MOVEMENTSPEED * frameTime;
        }
        else if (playerChara.getX() + playerChara.getImagePtr()->getWidth() <= GAME_WIDTH)
        {
            playerChara.setX(playerChara.getX() + MOVEMENTSPEED * frameTime);
        }
    }

    // Left Key
    if (dxManager->getInput()->isKeyDown(NAVI_LEFT_KEY))
    {
        if (playerChara.getX() <= GAME_WIDTH / 2 - playerChara.getImagePtr()->getWidth() / 2 && worldX < 0)
        {
            worldX += MOVEMENTSPEED * frameTime;
        }
        else if(playerChara.getX() >= 0)
        {
            playerChara.setX(playerChara.getX() - MOVEMENTSPEED * frameTime);
        }
    }

    // Down Key
    if (dxManager->getInput()->isKeyDown(NAVI_DOWN_KEY))
    {
        if (playerChara.getY() >= GAME_HEIGHT / 2 - playerChara.getImagePtr()->getHeight() / 2 && -worldY + GAME_WIDTH <= (worldMap.getHeight()))
        {

            worldY -= MOVEMENTSPEED * frameTime;

        }
        else if(playerChara.getY() + playerChara.getHeight() <= GAME_HEIGHT)
        {
            playerChara.setY(playerChara.getY() + MOVEMENTSPEED * frameTime);
        }
    }

    // Up Key
    if (dxManager->getInput()->isKeyDown(NAVI_UP_KEY))
    {
        if (playerChara.getY() <= GAME_HEIGHT / 2 - playerChara.getImagePtr()->getHeight() / 2 && worldY < 0)
        {

            worldY += MOVEMENTSPEED * frameTime;

        }
        else if (playerChara.getY() >= 0)
        {
            playerChara.setY(playerChara.getY() - MOVEMENTSPEED * frameTime);
        }
    }
}

//=====================================================================================
//          worldX : x == windowX : (GameWidth/2 - worldWidth/2) + x
//          worldY : y == windowY : (GameHeight/2 - worldHeight/2) + y
//=====================================================================================
void Overworld::setWorldPosition()
{
    //Setting the world map position
    worldMap.setX(worldX);
    worldMap.setY(worldY);

    enemyChara.setX(worldX + GAME_WIDTH / 2);
    enemyChara.setY(worldY + 1840);
}