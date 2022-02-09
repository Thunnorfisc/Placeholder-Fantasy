//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is CPP file for EntityManager. This is the older version of the EntityManager.
//				  This version uses one list and the "Zvalues" is the index in the list, Except 
//				  the larger the value the more forward it will be.
//===============================================================================================

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
	ent->setLayer(layer.size() - 1);
}

void EntityManager::update(float frameTime)
{
	for(int i = 0; i < layer.size();i++)
	{
		layer.at(i)->setLayer(i);
	}
}

//Insert somewhere in the list
void EntityManager::insert(Entity* ent, int index)
{
	if (index <= layer.size())
	{
		layer.insert(layer.begin() + index, ent);

		for (int i = 0; i < layer.size(); i++)
		{
			layer.at(i)->setLayer(i);
		}
	}
	else return;
}

//remove a Entity
void EntityManager::remove(int index)
{
	if (index < layer.size()) layer.erase(layer.begin() + index);
	for (int i = 0; i < layer.size(); i++)
	{
		layer.at(i)->setLayer(i);
	}
}

////replace 2 entity position in the list
//void EntityManager::replace(int originalIndex, int newIndex)
//{
//	if (newIndex < layer.size()) {
//		Entity* entity1 = layer.at(newIndex);
//		layer.at(newIndex) = layer.at(originalIndex);
//		layer.at(originalIndex) = entity1;
//	}
//}