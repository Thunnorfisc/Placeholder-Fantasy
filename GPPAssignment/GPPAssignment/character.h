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

	virtual bool initializeTextures(Game* gameptr, const char* OverworldTexture, const char* BattleTexture);

	virtual void update(float frameTime);

	virtual TextureManager* getTexture() { return currentTexture; }

	virtual bool switchTexture();

	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);
};
#endif // !_CHARACTER_H_
