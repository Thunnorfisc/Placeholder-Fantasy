#ifndef _ENTITYMANAGERV2_H_
#define _ENTITYMANAGERV2_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"
#include <vector>

class EntityManagerV2
{
private:
	int size;
	std::vector<std::vector<Entity*>*> listOfLayers;

public:
	EntityManagerV2();
	~EntityManagerV2();

	bool Initialize(int numberOfLayers);

	std::vector<std::vector<Entity*>*>  GetLayers() { return listOfLayers; }
	bool AddToLayer(Entity* ent, int layer);

	bool RemoveFromLayer(Entity* ent, int layer);
	
	int Size() { return size; }
};

#endif // !_ENTITYMANAGERV2_H_