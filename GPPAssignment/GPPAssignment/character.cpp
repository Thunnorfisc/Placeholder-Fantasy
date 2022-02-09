//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is CPP file for Character. It is a child of Entity. All characters in  
//                the game is defaulted to Character. While they contain the information for the
//				  battle system, it is not being used as it is not complete.
//===============================================================================================
#include "Character.h"

Character::Character()
{
	health = 0;
	mana = 0;
	speed = 0;

	currentPosition = 0;

	currentTexture = NULL;

}

bool Character::initialize(Game *gameptr,const char *OverworldTexture = NULL, const char* BattleTexture = NULL, int width = 0, int height = 0, int ncol = 0)
{
	if(OverworldTexture != NULL)
		overworldTexture.initialize(gameptr->getGraphics(), OverworldTexture);
	if(BattleTexture != NULL)
		battleTexture.initialize(gameptr->getGraphics(), BattleTexture);

	currentTexture = &overworldTexture;

	setClass("Character");

	Entity::initialize(gameptr, width, height, ncol, currentTexture);

	return true;
}
bool Character::initialize(Game *gameptr,const char *OverworldTexture = NULL, const char* BattleTexture = NULL)
{
	if(OverworldTexture != NULL)
		overworldTexture.initialize(gameptr->getGraphics(), OverworldTexture);
	if(BattleTexture != NULL)
		battleTexture.initialize(gameptr->getGraphics(), BattleTexture);

	currentTexture = &overworldTexture;


	setClass("Character");

	Entity::initialize(gameptr, 0, 0, 0, currentTexture);

	return true;
}

bool Character::switchTexture()
{
	if (currentTexture == &overworldTexture)
	{
		currentTexture = &battleTexture;
		return true;
	}
	else if (currentTexture == &battleTexture)
	{
		currentTexture = &overworldTexture;
		return true;
	}
	else
	{
		return false;
	}
}

bool Character::collideBox(Entity& ent, VECTOR2& collisionVector)
{
	if (!active || !ent.getActive()) return false;

    // Check for collision using Axis Aligned Bounding Box.
    if ((getCenterX() + edge.right * getScaleX() < ent.getCenterX() + ent.getEdge().left * ent.getScaleX()) ||
        (getCenterX() + edge.left * getScaleX() > ent.getCenterX() + ent.getEdge().right * ent.getScaleX()) ||
        (getCenterY() + edge.bottom * getScaleY() < ent.getCenterY() + ent.getEdge().top * ent.getScaleY()) ||
        (getCenterY() + edge.top * getScaleY() > ent.getCenterY() + ent.getEdge().bottom * ent.getScaleY()))
    {
        return false;
    }

    // set collision vector
    collisionVector = *ent.getCenter() - *getCenter();
    return true;
}

bool Character::collideBox(Entity* ent, VECTOR2& collisionVector)
{
	return false;
}

void Character::update(float frameTime)
{

	return Entity::update(frameTime);
}