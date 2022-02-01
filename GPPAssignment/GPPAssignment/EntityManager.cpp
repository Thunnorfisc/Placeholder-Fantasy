#include "EntityManager.h"

EntityManager::EntityManager() 
{
	layer = std::vector<Entity*>();
}

EntityManager::~EntityManager() {}

void EntityManager::push(Entity* ent)
{
	layer.push_back(ent);
}

void EntityManager::insert(Entity* ent, int index)
{
	if (index < layer.size()) layer.insert(layer.begin()+index, ent);
	else return;
}

void EntityManager::remove(int index)
{
	if (index < layer.size()) layer.erase(layer.begin() + index);
}

void EntityManager::replace(int originalIndex, int newIndex)
{
	if (newIndex < layer.size()) {
		Entity* entity1 = layer.at(newIndex);
		layer.at(newIndex) = layer.at(originalIndex);
		layer.at(originalIndex) = entity1;
	}
}