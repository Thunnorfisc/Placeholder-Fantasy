//===============================================================================================
//	Module		: Game Programming
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is the overworld scene that will be accessed from both menu and battle.
//				  Here the code will be handling movement by making use of WorldX and Y to move
//				  the world instead of the player character, to simulate a camera object. The 
//				  other object in the world's position will be relative to World X and Y and World
//                X and Y will be edited to simulate the movement instead of player character getX
//                and set X.
//                William worked of the boundary check for the world / camera and Jason worked on
//                the boundary check for the player character.
//===============================================================================================


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
    player.initializeTextures(dxManager, MAINCHARA_ANIMATION, MAINCHARA_IMAGE);
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
    // Goes to pause menu
    if (dxManager->getInput()->wasKeyPressed(VK_ESCAPE)) 
    {
        // Store world coordinates into game state
        //dxManager->getState()->setValueToState("WorldX", worldX);
        //dxManager->getState()->setValueToState("WorldY", worldY);
        dxManager->switchScene("PauseMenu");
    }
}

void Overworld::ai()
{

}
//======================================================================
//				Collision to switch to BattleScene
//======================================================================
void Overworld::collisions()
{
    VECTOR2 collisionVector;

    //if (playerChara.collidesWith(enemyChara, collisionVector))
    //{
    //    enemyChara.setActive(false);
    //    playerChara.setActive(false);
    //    dxManager->switchScene("Battle", characterList);
    //}
}

//=============================================================================
//                          Render game items
//=============================================================================
void Overworld::render()
{
    dxManager->getGraphics()->spriteBegin();

    player.draw(TRANSCOLOR);
    //Draw the world map
    //worldMap.draw(TRANSCOLOR);

    //renderCharacters();
    dxManager->getGraphics()->spriteEnd();
}

//=============================================================================
// The dxManager->getGraphics() device was lost.
// Release all reserved video memory so dxManager->getGraphics() device may be reset.
//=============================================================================
void Overworld::releaseAll()
{
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void Overworld::resetAll()
{

    return;
}

//Initialize the world and set default position
//void Overworld::initializeWorld()
//{
//    //initializing World map Texture
//    if (!worldMapTexture.initialize(dxManager->getGraphics(), OVERWORLD_IMAGE))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize World Map initialize"));
//
//    //initializing World Map
//    if (!worldMap.initialize(dxManager->getGraphics(), 0, 0, 0, &worldMapTexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize World Map"));
//
//    //Scaling Up world
//    worldMap.setScale(3, 3);
//
//    //If map stuff doesn't exist, tell it to go to default
//    if ((dxManager->getState()->isExistInState("WorldX") == false) && (dxManager->getState()->isExistInState("WorldY") == false))
//    {
//        dxManager->getState()->setValueToState("WorldX", -worldMap.getWidth() / 2 + GAME_WIDTH / 2);
//        dxManager->getState()->setValueToState("WorldY", -worldMap.getHeight() / 2 + GAME_HEIGHT / 2);
//    }
//
//    //Setting Default Position of the world
//    worldMap.setX(dxManager->getState()->getFloatFromState("WorldX"));
//    worldMap.setY(dxManager->getState()->getFloatFromState("WorldY"));
//
//    //Setting world parameters
//    worldX = worldMap.getX();
//    worldY = worldMap.getY();
//}


//=====================================================================================
//              Set Main Character to always be center of the screen
//            Rest of the Characters are set base on WorldX and WorldY
// 
//            E.g. test.setX(WorldX + X), X being the world cooridinate
//=====================================================================================
//void Overworld::initializeCharacters() 
//{
//    //initializing Character Texture
//    if (!mainCharaTexture.initialize(dxManager->getGraphics(), MAINCHARA_IMAGE))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Main Character texture"));
//    
//    if (!animationTexture.initialize(dxManager->getGraphics(), MAINCHARA_ANIMATION))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Animation texture"));
//
//    //initializing player character
//    if (!playerChara.initialize(dxManager, 34, 34, 1, &animationTexture, "Ayame","Hero",100,true))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Main Character"));
//    
//    //Setting Scale
//    playerChara.setScale(3, 3);
//    playerChara.getImagePtr()->setFrame(0, 4);
//
//    //Setting Default Position of the world
//    playerChara.setX(GAME_WIDTH / 2 - playerChara.getImagePtr()->getWidth() / 2);
//    playerChara.setY(GAME_HEIGHT / 2 - playerChara.getImagePtr()->getHeight() / 2);
//
//    characterList.push_back(playerChara);
//
//    //initializing enemyCharacter
//    if (!enemyChara.initialize(dxManager, 0, 0, 0, &mainCharaTexture, "WoD", "Tank", 100))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize enemyCharacter"));
//
//    //Getting the right orientationa and scale
//    enemyChara.setScale(1.5, 1.5);
//    enemyChara.getImagePtr()->flipHorizontal(true);
//
//    //Setting Enemy position
//    enemyChara.setX(worldX + GAME_WIDTH / 2);
//    enemyChara.setY(worldY + 1840);
//
//}



//=====================================================================================
//                                  Movement Controls
//                              Updates WorldX and WorldY
//=====================================================================================
void Overworld::controls(float frameTime) {
    // Right Key
    if (dxManager->getInput()->isKeyDown(NAVI_RIGHT_KEY))
    {
        playerChara.getImagePtr()->flipHorizontal(true);
        playerChara.getImagePtr()->update(3*frameTime);
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
        playerChara.getImagePtr()->flipHorizontal(false);
        playerChara.getImagePtr()->update(3 * frameTime);
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
        playerChara.getImagePtr()->update(3 * frameTime);
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
        playerChara.getImagePtr()->update(3 * frameTime);
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

// Neccessary Evil Why Please God No More LNK2001 Errors
void Overworld::onMessage(const Mail& mail)
{ }
//void Overworld::controls(float frameTime) {
//    // Right Key
//    if (dxManager->getInput()->isKeyDown(NAVI_RIGHT_KEY))
//    {
//        playerChara.getImagePtr()->flipHorizontal(true);
//        playerChara.getImagePtr()->update(3*frameTime);
//        if (playerChara.getX() >= GAME_WIDTH / 2 - playerChara.getWidth() / 2 && -worldX + GAME_WIDTH <= (worldMap.getWidth()))
//        {
//            worldX -= MOVEMENTSPEED * frameTime;
//        }
//        else if (playerChara.getX() + playerChara.getImagePtr()->getWidth() <= GAME_WIDTH)
//        {
//            playerChara.setX(playerChara.getX() + MOVEMENTSPEED * frameTime);
//        }
//    }
//
//    // Left Key
//    if (dxManager->getInput()->isKeyDown(NAVI_LEFT_KEY))
//    {
//        playerChara.getImagePtr()->flipHorizontal(false);
//        playerChara.getImagePtr()->update(3 * frameTime);
//        if (playerChara.getX() <= GAME_WIDTH / 2 - playerChara.getImagePtr()->getWidth() / 2 && worldX < 0)
//        {
//            worldX += MOVEMENTSPEED * frameTime;
//        }
//        else if(playerChara.getX() >= 0)
//        {
//            playerChara.setX(playerChara.getX() - MOVEMENTSPEED * frameTime);
//        }
//    }
//
//    // Down Key
//    if (dxManager->getInput()->isKeyDown(NAVI_DOWN_KEY))
//    {
//        playerChara.getImagePtr()->update(3 * frameTime);
//        if (playerChara.getY() >= GAME_HEIGHT / 2 - playerChara.getImagePtr()->getHeight() / 2 && -worldY + GAME_WIDTH <= (worldMap.getHeight()))
//        {
//
//            worldY -= MOVEMENTSPEED * frameTime;
//
//        }
//        else if(playerChara.getY() + playerChara.getHeight() <= GAME_HEIGHT)
//        {
//            playerChara.setY(playerChara.getY() + MOVEMENTSPEED * frameTime);
//        }
//    }
//
//    // Up Key
//    if (dxManager->getInput()->isKeyDown(NAVI_UP_KEY))
//    {
//        playerChara.getImagePtr()->update(3 * frameTime);
//        if (playerChara.getY() <= GAME_HEIGHT / 2 - playerChara.getImagePtr()->getHeight() / 2 && worldY < 0)
//        {
//
//            worldY += MOVEMENTSPEED * frameTime;
//
//        }
//        else if (playerChara.getY() >= 0)
//        {
//            playerChara.setY(playerChara.getY() - MOVEMENTSPEED * frameTime);
//        }
//    }
//}
//
////=====================================================================================
////          worldX : x == windowX : (GameWidth/2 - worldWidth/2) + x
////          worldY : y == windowY : (GameHeight/2 - worldHeight/2) + y
////=====================================================================================
//void Overworld::setWorldPosition()
//{
//    //Setting the world map position
//    worldMap.setX(worldX);
//    worldMap.setY(worldY);
//
//    enemyChara.setX(worldX + GAME_WIDTH / 2);
//    enemyChara.setY(worldY + 1840);
//}
