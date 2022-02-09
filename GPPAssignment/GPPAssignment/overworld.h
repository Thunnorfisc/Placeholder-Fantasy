//===========================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Name		: Jason Teo Jun Hao
//	S/N			: S10194978
//	Description	: This header file is for the overworld scene that handles movement and
//				  collision between the player character and the enemies in the overworld.
//				  It stores the variables such as worldX and Y to be used to handle movement
//                in the game and placements of objects in the world. The character list is
//                used in the initiating of the BattleScene. (This is a Improved Version)
//===========================================================================================

#ifndef _OVERWORLD_H_             // prevent multiple definitions if this 
#define _OVERWORLD_H_             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "sceneManager.h"
#include "EntityManager.h"
#include "EntityManagerV2.h"
#include "Player.h"
#include "World.h"
#include "NPC.h"
#include "Interactable.h"
#include "audio.h"

using namespace std;

class Overworld : public Scene
{
protected:
    SceneManager* dxManager;

    EntityManagerV2 entManagerV2;

    //Player Objects
    Player player;

    //NPCs
    NPC npc1;

    //Things to collide with
    Interactable cliff_1;
    Interactable cliff_2;
    Interactable cliff_3;
    Interactable wagon;
    Interactable waterfall;
    Interactable TPOBJECT;

    //World objects
    World world;
    float worldVelocityX;
    float worldVelocityY;

    float screenWidth, screenHeight;
    float WorldX, WorldY;

    VECTOR2 collisionVector;
public:
    // Constructor
    Overworld(SceneManager* manager);

    // Destructor
    virtual ~Overworld();

    //Initialize world
    void initialize();

    //Override virtual in from Game
    void reset() { return; };
    void update(float frameTime);
    void ai();
    void collisions();
    void render();
    void releaseAll();
    void resetAll();
    void onMessage(const Mail& mail);

    /// <summary>
    /// Set the World Pos X and Y
    /// </summary>
    void setWorldPos(float x, float y, World* world);
    float getWorldX() { return WorldX; }
    float getWorldY() { return WorldY; }

    /// <summary>
    /// Update the gameobjects position accordingly to the world X and Y
    /// </summary>
    void updateWorldObj(Entity* ent);
};
#endif // !_OVERWORLD_H