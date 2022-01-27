#ifndef _INTERACTABLE_H_
#define _INTERACTABLE_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"

class Interactable : public Entity
{
protected:

public:
	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);
};
#endif // !_Interactable_H_
