//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is the StartingRoom scene that will be accessed from Main Menu. 
//                This is to show that the player will move if the world is smaller or equal to
//                the Screen size. This makes use of EntityManagerV2 as its layering system.
//===============================================================================================


#include "StartingRoom.h"
#include <iostream>
using namespace std;

StartingRoom::StartingRoom(SceneManager* manager) {
    dxManager = manager;
    screenHeight = GAME_HEIGHT;
    screenWidth = GAME_WIDTH;
    WorldX = 0;
    WorldY = 0;
}

StartingRoom::~StartingRoom()
{
    releaseAll();
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void StartingRoom::initialize()
{
    //initialize layering system
    if (!entManagerV2.Initialize(MAXLAYERS)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize layering system"));

    //Initializing starting world
    if (!world.initialize(dxManager, BEDROOM_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR,"Failed to initialize world"));
    
    //Initialize starting World Coordinates
    setWorldPos(screenWidth / 2 - world.getWidth() / 2.0, screenHeight / 2 - world.getHeight() / 2.0);
    
    //Set world parameters
    world.setX(getWorldX());
    world.setY(getWorldY());
    world.BoundsSetup(world.getWidth() * 0.75, world.getWidth() * 0.25, world.getHeight() * 0.9, world.getHeight() * 0.1);

    //initialize teleportation
    if (!tpObject.initialize(dxManager, NULL)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize world's teleportation"));

    //Teleportation Object Variables
    tpObject.setSceneManager(dxManager);
    tpObject.setSize(BEDROOM_TP_WIDTH,BEDROOM_TP_HEIGHT);
    tpObject.setX(((world.retrieveBound()->maxX + world.retrieveBound()->minX) * 0.5) - tpObject.getWidth() / 2);
    tpObject.setY(world.retrieveBound()->maxY - tpObject.getHeight()/2);
    tpObject.setTag("tpObj");
    tpObject.setDestination("Overworld");
    if(!entManagerV2.AddToLayer(&tpObject, 0)) throw(GameError(gameErrorNS::FATAL_ERROR, "Invalid index when adding tpObject"));

    //Initialize bed
    if (!bed.initialize(dxManager, BED_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize bed object"));

    //Bed Variables
    bed.setX(screenWidth* 0.27);
    bed.setY(screenHeight * 0.1);

    if(!entManagerV2.AddToLayer(&bed, entManagerV2.Size() - 2)) throw(GameError(gameErrorNS::FATAL_ERROR, "Invalid index when adding Bed Objects"));

    //Initializing Players
    if(!player.initialize(dxManager, MAINCHARA_ANIMATION, MAINCHARA_IMAGE, CHARA_SIZE, CHARA_SIZE,0)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize player"));

    //Player Variables
    player.setScale(CHARA_SCALE, CHARA_SCALE);
    player.setX(screenWidth/2 - player.getWidth()/2.0);
    player.setY(screenHeight/2 - player.getHeight()/2.0);
    player.setClass("Player");
    player.setFrame(0, 4);

    //Add the entities to the lists
    if (!entManagerV2.AddToLayer(&player, entManagerV2.Size() - 3)) throw(GameError(gameErrorNS::FATAL_ERROR, "Invalid index when adding Player Objects"));
    //entManager.push(&player);
    return;
}

void StartingRoom::reset()
{
    return;
}

//=============================================================================
// Update to check for game logic
// frameTime is used to regulate the speed of movement
//=============================================================================
void StartingRoom::update(float frameTime)
{
    // Goes to pause menu
    if (dxManager->getInput()->wasKeyPressed(VK_ESCAPE)) 
    {
        // Store world coordinates into game state
        //dxManager->getState()->setValueToState("WorldX", worldX);
        //dxManager->getState()->setValueToState("WorldY", worldY);
        dxManager->layerScene("PauseMenu");
    }

    //V2 TriggerLayers
    for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
    {
        for (int i = 0; i < layer->size(); i++)
        {
            layer->at(i)->update(frameTime);
        }
    }
}

void StartingRoom::ai()
{

}
//======================================================================
//				Collision to switch to BattleScene
//======================================================================
void StartingRoom::collisions()
{
    VECTOR2 collisionVector;

    world.CheckBoundsCollision(&player);
    world.CheckCameraBound(&player, WorldX, WorldY);

    //V2 TriggerLayers
    for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
    {
        for (int i = 0; i < layer->size(); i++)
        {
            if (layer->at(i)->getClass() == "Interactable")
            {
                Interactable* interactable = dynamic_cast<Interactable*> (layer->at(i));
                interactable->triggerLayerV2(&player, &entManagerV2);
                interactable->collideBox(&player, collisionVector);
                interactable->triggerBox(&player, collisionVector);
            }
        }
    }
}

//=============================================================================
//                          Render game items
//=============================================================================
void StartingRoom::render()
{
    dxManager->getGraphics()->spriteBegin();

    //Draw the world map
    world.draw(TRANSCOLOR);

    //Here is where the layer system go

    //V2 System
    for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
    {
        for (int i = 0; i < layer->size(); i++)
        {
            layer->at(i)->draw(TRANSCOLOR);
        }
    }

    ////V1 System
    //for (Entity* ent : entManager.retrieveLayers())
    //{
    //    ent->draw(TRANSCOLOR);
    //}
    
    //worldMap.draw(TRANSCOLOR);

    //renderCharacters();
    dxManager->getGraphics()->spriteEnd();
}

//=============================================================================
// The dxManager->getGraphics() device was lost.
// Release all reserved video memory so dxManager->getGraphics() device may be reset.
//=============================================================================
void StartingRoom::releaseAll()
{
    for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
    {
        world.getTextureManager()->onLostDevice();
        for (int i = 0; i < layer->size(); i++)
        {
            layer->at(i)->getTextureManager()->onLostDevice();
        }
    }
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void StartingRoom::resetAll()
{
    for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
    {
        world.getTextureManager()->onResetDevice();
        for (int i = 0; i < layer->size(); i++)
        {
            layer->at(i)->getTextureManager()->onResetDevice();
        }
    }
    return;
}

// Neccessary Evil Why Please God No More LNK2001 Errors
void StartingRoom::onMessage(const Mail& mail)
{ }

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
