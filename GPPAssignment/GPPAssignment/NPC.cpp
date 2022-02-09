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