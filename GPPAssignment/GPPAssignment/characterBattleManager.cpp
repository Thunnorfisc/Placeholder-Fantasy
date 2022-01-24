////===============================================================================================
////	Module		: Game Programming
////	Assignment	: Placeholder Fantasy?	
////	Author		: Jason Teo Jun Hao
////	Student ID	: S10194978B
////	Description	: This is the Character Battle Manager cpp file. It is used to set the positions
////				  of the characters to their respective positions. The positions are predetermined
////				  as a Position Struct and are initialized in the contructor method. The characters
////				  will then be assigned based on the teams they are in and their index.
////===============================================================================================
//
//
//#include "characterBattleManager.h"
//
//CharacterBattleManager::CharacterBattleManager()
//{
//
//	friendlyPositions.push_back({ GAME_WIDTH * (2.0 / 3.0), GAME_HEIGHT * (9.0 / 16.0) });
//	friendlyPositions.push_back({ GAME_WIDTH * (8.0 / 9.0), GAME_HEIGHT * (9.0 / 16.0) });
//	friendlyPositions.push_back({ GAME_WIDTH * (7.0 / 9.0), GAME_HEIGHT * (15.0 / 32.0) });
//	friendlyPositions.push_back({ GAME_WIDTH * (7.0 / 9.0), GAME_HEIGHT * (21.0 / 32.0) });
//
//	enemyPositions.push_back({ GAME_WIDTH * (1.0 / 3.0), GAME_HEIGHT * (9.0 / 16.0) });
//	enemyPositions.push_back({ GAME_WIDTH * (1.0 / 9.0), GAME_HEIGHT * (9.0 / 16.0) });
//	enemyPositions.push_back({ GAME_WIDTH * (2.0 / 9.0), GAME_HEIGHT * (15.0 / 32.0) });
//	enemyPositions.push_back({ GAME_WIDTH * (2.0 / 9.0), GAME_HEIGHT * (21.0 / 32.0) });
//
//
//	
//}
//
//CharacterBattleManager::~CharacterBattleManager()
//{
//	
//}
//
//void CharacterBattleManager::initialize(std::vector<Character>* newList)
//{
//	enemycount = 0;
//	friendcount = 0;
//
//	listOfCharacters = newList;
//
//	for (int i = 0; i < listOfCharacters->size(); i++)
//	{
//		listOfCharacters->at(i).getImagePtr()->setScale(2, 2);
//
//		float characterWidth = listOfCharacters->at(i).getImagePtr()->getWidth() / 2;
//		float characterHeight = listOfCharacters->at(i).getImagePtr()->getHeight() / 2;
//
//		if (listOfCharacters->at(i).getFriendlyTag())
//		{
//			listOfCharacters->at(i).setX(friendlyPositions[friendcount].x - characterWidth);
//			listOfCharacters->at(i).setY(friendlyPositions[friendcount].y - characterHeight);
//			friendcount++;
//		}
//		else
//		{
//			listOfCharacters->at(i).setX(enemyPositions[enemycount].x - characterWidth);
//			listOfCharacters->at(i).setY(enemyPositions[enemycount].y - characterHeight);
//			enemycount++;
//		}
//	}
//}
//
//void CharacterBattleManager::draw()
//{
//	for (int i = 0; i < listOfCharacters->size(); i++)
//	{
//		if(!listOfCharacters->at(i).getFriendlyTag())listOfCharacters->at(i).draw(graphicsNS::RED);
//		else listOfCharacters->at(i).draw();
//	}
//}
