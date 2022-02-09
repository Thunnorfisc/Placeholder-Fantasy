//============================================================================<<
//  Module:             Gameplay Programming
//  Assignment 2:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================<>
// Description of MenuOption:
// This is a struct used for each menu option in the menu. 
// This makes coding a lot easier as now the string, 
//		x and y values can all be at one
// data type that is easily accessible. Mostly used in menus.
//============================================================================>>
#ifndef _MENUOPTION_H             // prevent multiple definitions if this 
#define _MENUOPTION_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <list>
#include <string>

struct MenuOption {
	std::string option;  // Name and Display of MenuOption
	int x;               // X coordinate of option
	int y;               // Y coordinate of option
};
#endif
