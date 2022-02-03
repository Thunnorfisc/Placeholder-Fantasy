#ifndef _INTERACTABLE_H_
#define _INTERACTABLE_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"

class Interactable : public Entity
{
protected:
	TextureManager intTexture;

public:
	Interactable();

	bool initialize(Game* gameptr, const char* texture);

	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);

	virtual void update(float frameTime);
};
#endif // !_Interactable_H_
