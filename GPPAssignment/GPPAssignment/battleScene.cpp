//===============================================================================================
//	Module		: Game Programming
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is the battle scene that is accessed by encountering an enemy in the
//				  overworld. The character list is passed through from the overworld and is
//				  determined by the group of enemies a npc represents and vice versa for the
//				  player character
//===============================================================================================

#include <time.h>
#include "battleScene.h"

//Constructor
BattleScene::BattleScene(SceneManager* manager) {
	dxManager = manager;

	characterBM = CharacterBattleManager();

	selectionY = 0;
	attackPhase = 1;
}

//Destructor
BattleScene::~BattleScene()
{
	releaseAll();			//call onLostDevice() for every dxManager->getGraphics() item

}

//Initialize the Scene
void BattleScene::initialize()
{
	battleUI.setGameState();

	attackPhase = 1;
	selectionY = 0;
	//Allow for the randomizer for the damage to be always different
	srand(time(NULL));

	//Assign the totalCharacterScenes to the BattleScene CharacterList
	characterList = dxManager->getCharacterList();

	//Initializing Floor
	if (!placeholderRectTexture.initialize(dxManager->getGraphics(), PLACEHOLDERRECT))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Place holder rect"));
	if (!floor.initialize(dxManager->getGraphics(), 0,GAME_HEIGHT/2,0,&placeholderRectTexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize floor"));
	if (!sky.initialize(dxManager->getGraphics(), 0,GAME_HEIGHT/2,0,&placeholderRectTexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize floor"));
	
	
	floor.setY(GAME_HEIGHT/2);

	//Initialize the BattleUI
	battleUI.initialize(dxManager, characterList);
	characterBM.initialize(characterList);

	return;
}

//Update all game items
void BattleScene::update(float frameTime) {

	//Call BattleUI Update to update the UI
	battleUI.update(dxManager, frameTime);

	optionChosen = battleUI.checkSelection(dxManager);

	turnSystem();
}

//Artificial Intelligence
void BattleScene::ai() {

}

//Handle collisions
void BattleScene::collisions() {}

//Render game items
void BattleScene::render() {
	dxManager->getGraphics()->spriteBegin();

	sky.draw(graphicsNS::SKYBLUE);

	floor.draw(graphicsNS::GREEN);

	battleUI.draw();

	characterBM.draw();

	dxManager->getGraphics()->spriteEnd();
}

//Check the selection string from BattleUI
void BattleScene::checkSelectionChosen(std::string option)
{
	ifDefend = 1;
	// Start -> Overworld
	if (option == "Attack")
	{
		//Attack
		characterList->at(1).deductHealth((rand() % 50) + 10);
		battleUI.StartChecking();
		attackPhase *= -1;
	}
	// Option -> Whatever options we have
	else if (option == "Defend")
	{
		//Defend Next Attack
		ifDefend = double(rand()) / double(RAND_MAX);
		battleUI.StartChecking();
		attackPhase *= -1;
	}
	// Quit -> Quit the game
	else if (option == "Run")
	{
		//Run From Battle
		dxManager->switchScene("Overworld");
	}
}

//Turn system changes by checking for state of animation and game state for gameover.
void BattleScene::turnSystem() {
	if (!battleUI.isChecking())
	{
		if (!battleUI.getGameState()) {
			if (attackPhase == 1) checkSelectionChosen(optionChosen);

			else if (attackPhase == -1)
			{
				characterList->at(0).deductHealth(((rand() % 40) + 10) * ifDefend);
				battleUI.StartChecking();
				attackPhase *= -1;
			}
		}
		else {
			dxManager->switchScene("Overworld");
		}
	}
}

//The dxManager->getGraphics() device was lost
//Release all reserved video memory to be resetted
void BattleScene::releaseAll() {
	
	battleUI.releaseAll();
	return;
}

//The dxManager->getGraphics() device has been reset.
//Recreate all reserved video memory to reset dxManager->getGraphics() device
void BattleScene::resetAll() {
	
	battleUI.resetAll();
	return;
}

// Reset
void BattleScene::reset()
{
	return;
}

// Neccessary Evil Why Please God No More LNK2001 Errors
void BattleScene::onMessage(const Mail& mail)
{ }
