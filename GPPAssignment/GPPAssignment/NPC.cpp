//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is CPP file for NPC. Incomplete
//===============================================================================================
#include "NPC.h"

NPC::NPC()
{
	friendly = true;
	interactable = true;
	dialogLines = std::vector<string>();
}

bool NPC::initialize()
{
	return true;
}

void NPC::update(float frameTime)
{
	Character::update(frameTime);
}