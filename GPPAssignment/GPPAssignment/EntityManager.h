#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"
#include <vector>

class EntityManager
{
private:
	std::vector<Entity*> layer;

public:
	EntityManager();
	~EntityManager();

	std::vector<Entity*> retrieveLayers() { return layer; }

	void push(Entity* ent);
	void insert(Entity& ent, int index);
	void remove(int index);
	void replace(int originalIndex, int newIndex);
};
#endif // !_ENTITYMANAGER_H_
