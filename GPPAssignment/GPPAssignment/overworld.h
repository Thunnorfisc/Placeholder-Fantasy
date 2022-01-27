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

#include <vector>
#include "game.h"
#include "textDX.h"
#include "constants.h"
#include "TextureManager.h"
#include "Image.h"
#include "sceneManager.h"
#include "gameState.h"

using namespace std;

class Overworld : public Scene
{
protected:
    SceneManager* dxManager;

    //Textures;
    TextureManager worldMapTexture;
    TextureManager animationTexture;
    TextureManager mainCharaTexture;

    //Images
    Image worldMap;

    //Characters
    Character playerChara;
    Character enemyChara;

    //World Parameters
    float worldX, worldY;

    std::vector<Character> characterList;

    int characterMovement;

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
    
    //If you are using this in the future please rename it to something that is relevant to what it is used for.
    //void checkMouse();

    void initializeWorld();

    void initializeCharacters();

    void controls(float frameTime);

    void setWorldPosition();
};
#endif // !_MAP_H