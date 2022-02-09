//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is H file for EntityManager. This is the older version of the EntityManager.
//				  This version uses one list and the "Zvalues" is the index in the list, Except 
//				  the larger the value the more forward it will be.
//===============================================================================================
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

	/// <summary>
	/// Retrieve the layers(Deprecated)
	/// </summary>
	/// <returns></returns>
	std::vector<Entity*> retrieveLayers() { return layer; }
	void update(float frameTime);

	/// <summary>
	/// Add to EntityManager layer system (Deprecated)
	/// </summary>
	/// <param name="ent"></param>
	void push(Entity* ent);

	/// <summary>
	/// Insert to a index in the layer system (Deprecated)
	/// </summary>
	/// <param name="ent"></param>
	/// <param name="index"></param>
	void insert(Entity* ent, int index);

	/// <summary>
	/// Remove object from the give index (Deprecated)
	/// </summary>
	/// <param name="index"></param>
	void remove(int index);
	//void replace(int originalIndex, int newIndex);
};
#endif // !_ENTITYMANAGER_H_
