//===========================================================================================
//	Module		: Game Programming
//	Assignment	: Placeholder Fantasy?	
//	Name		: Jason Teo Jun Hao
//	S/N			: S10194978
//	Description	: This header file is for the scene that will be handling the battle
//				  in the game. Here is supposed to make use of the BattleUI and
//				  CharacterBattleManager to deal with battle state / Menu selections
//				  and Displaying of any Images.
//===========================================================================================

#ifndef _PFANTASY_H
#define _PFANTASY_H
#define WIN32_LEAN_AND_MEAN

#include <vector>

#include "game.h"
#include "TextureManager.h"
#include "Image.h"
#include "character.h"
#include "battleUI.h"
#include "textDX.h"
#include "sceneManager.h"
#include "characterBattleManager.h"

class BattleScene : public Scene
{
private:
	//Reference to Scene Manager
	SceneManager* dxManager;

	//Components used to display elements for the Battle Scene
	BattleUI battleUI;
	CharacterBattleManager characterBM;

	//texture to be used for the sky and floor
	TextureManager placeholderRectTexture;

	//Character list to be used for the Battle
	std::vector<Character>* characterList;

	//Image objects for the floor and the sky
	Image floor;
	Image sky;

	//Ranges from 0 - 1, the lower the number, the less damage you take
	float ifDefend;

	//attack phase to be switched when friendly turn and enemy turn.
	int attackPhase;

	int selectionY;

	//Check for return string from BattleUI
	std::string optionChosen;

public:
	BattleScene(SceneManager* manager);					//Constructor
	virtual ~BattleScene();								//Destructor

	//initialize BattleScene
	void initialize();

	// Override virtual in from Game
	void update(float frameTime);
	void ai();
	void reset();
	void collisions();
	void render();

	// Check the Selection string returned by the BattleUI function
	void checkSelectionChosen(std::string option);

	// Handles the switch in turns
	void turnSystem();

	void releaseAll();
	void resetAll();
	void onMessage(const Mail& mail);

};
#endif // !_PFANTASY_H
