////===============================================================================================
////	Module		: Game Programming
////	Assignment	: Placeholder Fantasy?	
////	Author		: Jason Teo Jun Hao
////	Student ID	: S10194978B
////	Description	: This is the battle UI cpp file that will be used for processing the math for
////				  the placement of the different UI elements that will be used in the Battle Scene
////				  such as HealthBar and selection Objections. The code is supposed to be able to
////				  support up to 4 character each side and will scale and postition accordingly.
////===============================================================================================
//
//#include "battleUI.h"
//
//BattleUI::BattleUI() 
//{
//	selectionIndex = 0;
//	checking = false;
//	gameOver = false;
//	checkingIndex = 0;
//}
//
//BattleUI::~BattleUI() {}
//
//bool BattleUI::initialize(Game* gameptr, std::vector<Character>* listOfCharacters) 
//{
//	selectionIndex = 0;
//	checking = false;
//	gameOver = false;
//	checkingIndex = 0;
//
//	enemyTeam.clear();
//	friendlyTeam.clear();
//	enemyHpPosY.clear();
//	teamHpPosY.clear();
//	usedHealthBar.clear();
//
//	//Set List of Characters to be used in the Battle UI
//	listOfCharactersInPlay = listOfCharacters;
//	
//	//Assign the characters to their own teams
//	for (int i = 0; i < listOfCharactersInPlay->size(); i++)
//	{
//		if (listOfCharactersInPlay->at(i).getFriendlyTag()) friendlyTeam.push_back(listOfCharactersInPlay->at(i));
//
//		else enemyTeam.push_back(listOfCharactersInPlay->at(i));
//
//	}
//
//	//Initialize Textures
//	if (!UITexture.initialize(gameptr->getGraphics(), PLACEHOLDERRECT))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize UI Texture"));
//
//	//Intialize Fonts
//	if (!hpFont.initialize(gameptr->getGraphics(), 10, false,false, gameNS::INFOFONT));
//	if (!selectionFont.initialize(gameptr->getGraphics(), 25, false,false, gameNS::FONT));
//	selectionFont.setFontColor(graphicsNS::BLACK);
//
//	//Initializing Action Bar
//	if (!actionBar.initialize(gameptr->getGraphics(), 0, 0, 0, &UITexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize action bar"));
//	actionBar.setScaleY(0.25);
//	actionBar.setX(GAME_WIDTH / 2 - actionBar.getWidth() / 2);
//	actionBar.setY(GAME_HEIGHT - actionBar.getHeight());
//
//	//Set Health Bar Position
//	setHpPosYEnemy();
//	setHpPosYTeam();
//
//	//Set Up Selection Option
//	selectionMenuSetup(gameptr);
//	//Setup Health Bars
//	healthBarSetup(gameptr);
//
//	return true;
//}
//
////=========================================================
////			Update Function for BattleUI
////=========================================================
//void BattleUI::update(Game* gameptr, float frameTime)
//{
//	if (checking) // checking will be set to true when the function startChecking is called in BattleUI
//	{
//		if (listOfCharactersInPlay->at(checkingIndex).getCurrentHealth() <= 0) // When one of the character reaches 0 hp
//		{
//			updateHpBars(listOfCharactersInPlay->at(checkingIndex), frameTime / 5);
//			gameOver = true;
//		}
//
//		if (listOfCharactersInPlay->at(checkingIndex).getCurrentHealth() / listOfCharactersInPlay->at(checkingIndex).getMaxHealth() < usedHealthBar[listOfCharactersInPlay->at(checkingIndex).getName()].getScaleX() && usedHealthBar[listOfCharactersInPlay->at(checkingIndex).getName()].getScaleX() > 0)
//		{
//			//as long as the health bar scale is not the same as the ratio of the current health and max health, it will continue to check and reduce scale.
//			checking = true;
//
//			// Update Health Bar
//			updateHpBars(listOfCharactersInPlay->at(checkingIndex), frameTime / 5);
//		}
//		else
//		{
//			if (checkingIndex < listOfCharactersInPlay->size() - 1) checkingIndex++; // This ensures that all healthbars are updated. In case of AOE
//
//			else
//			{
//				checkingIndex = 0;
//				checking = false;
//			}
//			
//		}
//	}
//}
//
////=========================================================
////				Draw function of BattleUI
////=========================================================
//void BattleUI::draw()
//{
//	//Draw Action Bar
//	actionBar.draw(graphicsNS::LTGRAY);
//
//	//Draw Selection Options
//	selectionMenuPrint();
//
//	//Draw health bars
//	drawHealthBars();
//}
//
////=========================================================
////				Set Up HealthBars (Enemies)
////=========================================================
//void BattleUI::healthBarSetup(Game* gameptr)
//{
//	//Enemy Health Bars
//	for (int i = 0; i < enemyTeam.size(); i++)
//	{
//		if (!listOfHealth[i].initialize(gameptr->getGraphics(), HPBAR_WIDTH, HPBAR_HEIGHT, 1, &UITexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize HealthBar"));
//		listOfHealth[i].setX(GAME_WIDTH / 6 - listOfHealth[i].getWidth()/2);
//		listOfHealth[i].setY(enemyHpPosY[i]);
//		usedHealthBar[enemyTeam[i].getName()] = listOfHealth[i];
//	}
//
//	//Friendly Health Bars
//	for (int i = 0; i < friendlyTeam.size(); i++)
//	{
//		if (!listOfHealth[i + usedHealthBar.size()].initialize(gameptr->getGraphics(), HPBAR_WIDTH, HPBAR_HEIGHT, 1, &UITexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize HealthBar"));
//		listOfHealth[i + usedHealthBar.size()].setX(GAME_WIDTH / 6 * 3 - listOfHealth[i + usedHealthBar.size()].getWidth()/2);
//		listOfHealth[i + usedHealthBar.size()].setY(teamHpPosY[i]);
//		usedHealthBar[friendlyTeam[i].getName()] = listOfHealth[i + usedHealthBar.size()];
//	}
//}
//
////=========================================================
////		Set Up Y positions for HealthBars (Enemies)
////=========================================================
//void BattleUI::setHpPosYEnemy()
//{
//	float POI_Y = actionBar.getHeight() / (enemyTeam.size() + 1);
//	
//	for (int i = 0; i < enemyTeam.size(); i++)
//	{
//		enemyHpPosY.push_back(actionBar.getY() + POI_Y * (i + 1));
//	}
//}
//
////=========================================================
////		Set Up Y positions for HealthBars (Friendly)
////=========================================================
//void BattleUI::setHpPosYTeam()
//{
//	float POI_Y = actionBar.getHeight() / (friendlyTeam.size() + 1);
//
//	for (int i = 0; i < friendlyTeam.size(); i++)
//	{
//		teamHpPosY.push_back(actionBar.getY() + POI_Y * (i + 1));
//	}
//}
//
////======================================================================
////		Use to check for updates and update health bars if nessasary
////======================================================================
//bool BattleUI::updateHpBars(Character character, float frameTime)
//{
//	//Check for characters health decreasing
//	if (usedHealthBar[character.getName()].getScaleX() > 0) {
//		usedHealthBar[character.getName()].setScaleX(usedHealthBar[character.getName()].getScaleX() - 1 * frameTime);
//	}
//
//	return true;
//}
//
////======================================================================
////				Draw health bars | Called in draw()
////======================================================================
//
//void BattleUI::drawHealthBars() {
//
//	for (int i = 0; i < enemyTeam.size(); i++)
//	{
//		usedHealthBar[enemyTeam[i].getName()].draw(graphicsNS::RED);
//	}
//
//	for (int i = 0; i < friendlyTeam.size(); i++)
//	{
//		usedHealthBar[friendlyTeam[i].getName()].draw(graphicsNS::RED);
//	}
//}
//
////======================================================================
////						Setup Selection Menu
////======================================================================
//
//void BattleUI::selectionMenuSetup(Game* gameptr)
//{
//
//	int offset = 50;
//	//Display Selection Menu Text
//	int POI_Y = actionBar.getHeight() / 4;
//	selectionList.push_back(
//		{ 
//		"Attack", 
//		GAME_WIDTH - selectionFont.getWidth("Attack", selectionFont.getFont()) - offset,
//		int(actionBar.getY()) + POI_Y - selectionFont.getHeight("Attack", selectionFont.getFont()) / 2
//		}
//	);
//	
//	selectionList.push_back(
//		{ 
//		"Defend", 
//		GAME_WIDTH - selectionFont.getWidth("Defend", selectionFont.getFont()) - offset,
//		int(actionBar.getY()) + POI_Y * 2 - selectionFont.getHeight("Defend", selectionFont.getFont()) / 2
//		}
//	);
//
//	selectionList.push_back(
//		{
//		"Run", 
//		GAME_WIDTH - selectionFont.getWidth("Run", selectionFont.getFont()) - offset,
//		int(actionBar.getY()) + POI_Y * 3 - selectionFont.getHeight("Run", selectionFont.getFont()) / 2
//		}
//	);
//
//	if (!SelectionArrowTexture.initialize(gameptr->getGraphics(), Cursor))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Selection Arrow Texture"));
//	if (!selectionArrow.initialize(gameptr->getGraphics(),0,0,0,&SelectionArrowTexture))throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Selection Arrow"));
//	
//	selectionArrow.setX(selectionList[0].x - offset);
//	selectionArrow.setY(selectionList[0].y);
//}
//
////======================================================================
////					Displaying Selection Menu
////======================================================================
//void BattleUI::selectionMenuPrint()
//{
//	for (auto selection : selectionList)
//	{
//		selectionFont.print(selection.option, selection.x, selection.y);
//	}
//
//	selectionArrow.draw(TRANSCOLOR);
//}
//
////======================================================================
////						Check Selection Menu
////======================================================================
//std::string BattleUI::checkSelection(Game* gameptr)
//{
//	if (gameptr->getInput()->wasKeyPressed(VK_RETURN))
//	{
//		//Check option here
//		return selectionList[selectionIndex].option;
//	}
//
//	// Move down
//	if (gameptr->getInput()->wasKeyPressed(CURSOR_DOWN_KEY) && selectionIndex != (selectionList.size() - 1))
//	{
//		selectionIndex++; // Increases menu index to tell where the location of the cursor is
//		selectionArrow.setY(selectionList[selectionIndex].y);
//	}
//
//	// Move up
//	if (gameptr->getInput()->wasKeyPressed(CURSOR_UP_KEY) && selectionIndex != 0)
//	{
//		selectionIndex--; // Decreases menu index to tell where the location of the cursor is
//		selectionArrow.setY(selectionList[selectionIndex].y);
//	}
//
//	return "";
//}
//
//void BattleUI::releaseAll()
//{
//	UITexture.onLostDevice();
//}
//void BattleUI::resetAll() 
//{
//	UITexture.onResetDevice();
//}
//
