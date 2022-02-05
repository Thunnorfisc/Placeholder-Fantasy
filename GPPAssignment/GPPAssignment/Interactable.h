#ifndef _INTERACTABLE_H_
#define _INTERACTABLE_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"
#include "Player.h"
#include "EntityManager.h"
#include "EntityManagerV2.h"
#include <vector>

class Interactable : public Entity
{
protected:
	TextureManager intTexture;

public:
	Interactable();

	bool initialize(Game* gameptr, const char* texture);

	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);

	void triggerLayer(Player* player, EntityManager* layerSystem);

	void triggerLayerV2(Entity* player, EntityManagerV2* layerSystem);

	virtual void update(float frameTime);
};
#endif // !_Interactable_H_
