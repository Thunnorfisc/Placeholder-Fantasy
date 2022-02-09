//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is H file for Character. It is a child of Entity. All characters in  
//                the game is defaulted to Character. While they contain the information for the
//				  battle system, it is not being used as it is not complete.
//===============================================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"
//#include "Weapon.h"

class Character : public Entity
{
protected:
	//Parameters for battle scene
	float health;
	float mana;
	int speed;

	//currentPosition is used by all characters except enemies.
	int currentPosition;

	//Weapon *weapon

	TextureManager *currentTexture;
	TextureManager overworldTexture;
	TextureManager battleTexture;
	
public:
	Character();

	virtual bool initialize(Game* gameptr, const char* OverworldTexture, const char* BattleTexture);
	virtual bool initialize(Game* gameptr, const char* OverworldTexture, const char* BattleTexture, int width, int height, int ncol);

	virtual void update(float frameTime);

	virtual TextureManager* getTexture() { return currentTexture; }

	virtual bool switchTexture();

	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);
	virtual bool collideBox(Entity* ent, VECTOR2& collisionVector);

};
#endif // !_CHARACTER_H_
