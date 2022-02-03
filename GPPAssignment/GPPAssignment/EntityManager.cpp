#include "EntityManager.h"

//Constructor
EntityManager::EntityManager() 
{
	layer = std::vector<Entity*>();
}

EntityManager::~EntityManager() {}

//Add to the list
void EntityManager::push(Entity* ent)
{
	layer.push_back(ent);
}

//Insert somewhere in the list
void EntityManager::insert(Entity& ent, int index)
{
	if (index < layer.size()) layer.insert(layer.begin()+index, &ent);
	else return;
}

//remove a Entity
void EntityManager::remove(int index)
{
	if (index < layer.size()) layer.erase(layer.begin() + index);
}

//replace 2 entity position in the list
void EntityManager::replace(int originalIndex, int newIndex)
{
	if (newIndex < layer.size()) {
		Entity* entity1 = layer.at(newIndex);
		layer.at(newIndex) = layer.at(originalIndex);
		layer.at(originalIndex) = entity1;
	}
}