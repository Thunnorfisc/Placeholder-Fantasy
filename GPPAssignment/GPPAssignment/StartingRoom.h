//===========================================================================================
//	Module		: Game Programming
//	Assignment	: Placeholder Fantasy?	
//	Name		: Jason Teo Jun Hao
//	S/N			: S10194978
//	Description	: This header file is for the overworld scene that handles movement and
//				  collision between the player character and the enemies in the overworld.
//				  It stores the variables such as worldX and Y to be used to handle movement
//                in the game and placements of objects in the world. The character list is
//                used in the initiating of the BattleScene.
//===========================================================================================

#ifndef _STARTINGROOM_H             // prevent multiple definitions if this 
#define _STARTINGROOM_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "sceneManager.h"
#include "EntityManager.h"
#include "EntityManagerV2.h"
#include "Player.h"
#include "World.h"
#include "NPC.h"
#include "Interactable.h"

using namespace std;

class StartingRoom : public Scene
{
protected:
    SceneManager* dxManager;

    EntityManagerV2 entManagerV2;
    EntityManager entManager;

    Player player;

    Interactable bed;
    Interactable tpObject;

    World world;

    float screenWidth, screenHeight;
    float WorldX, WorldY;

public:
    // Constructor
    StartingRoom(SceneManager* manager);

    // Destructor
    virtual ~StartingRoom();

    //Initialize world
    void initialize();

    //Override virtual in from Game
    void reset();
    void update(float frameTime);
    void ai();
    void collisions();
    void render();
    void releaseAll();
    void resetAll();
    void onMessage(const Mail& mail);
    
    void setWorldPos(float x, float y) { WorldX = x; WorldY = y; }
    float getWorldX() { return WorldX; }
    float getWorldY() { return WorldY; }
};
#endif // !_MAP_H