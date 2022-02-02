#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"
#include "Character.h"
#include "Player.h"
#include <vector>

class EntityManager
{
private:
	std::vector<std::unique_ptr<Entity>> layer;

public:
	EntityManager();
	~EntityManager();

	std::vector<std::unique_ptr<Entity>> retrieveLayers() { return layer; }

	void push(Entity* ent);
	void insert(Entity* ent, int index);
	void remove(int index);
	void replace(int originalIndex, int newIndex);
};
#endif // !_ENTITYMANAGER_H_
