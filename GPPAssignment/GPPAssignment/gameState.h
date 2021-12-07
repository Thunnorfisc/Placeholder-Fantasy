//============================================================================
//  Module:             Gameplay Programming
//  Assignment 1:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_
#define WIN32_LEAN_AND_MEAN

#include <map>
#include <string>
#include <iostream>

struct Var
{
	std::string value;
	std::string type;
};
// This is the game state, keep tracks of global variable.
class GameState
{
private:
	std::map<std::string, Var> globalMap; // global variables
public:
	GameState();
	~GameState();

	// Getter and check if map entry exist: globalMap
	float getFloatFromState(std::string key);
	bool isFloatExist(std::string key);
	// Setter
	void setValueToState(std::string key, float var);
	// Clear map
	void resetState();
};

#endif 
