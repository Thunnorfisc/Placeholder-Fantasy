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

#ifndef _MAP_H             // prevent multiple definitions if this 
#define _MAP_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "sceneManager.h"
#include "Player.h"
#include "NPC.h"
#include "Interactable.h"

using namespace std;

class Overworld : public Scene
{
protected:
    SceneManager* dxManager;

    Player player;

public:
    // Constructor
    Overworld(SceneManager* manager);

    // Destructor
    virtual ~Overworld();

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
    
};
#endif // !_MAP_H