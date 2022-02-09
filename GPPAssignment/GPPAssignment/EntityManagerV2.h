//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is H file for EntityManagerV2. This is the newer version of the EntityManager.
//				  This version uses a nested list and the "Zvalues" is the list in the Layering System.
//				  Each list of Entities acts as a layer.
//===============================================================================================
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

	/// <summary>
	/// Initialize layering system
	/// </summary>
	/// <param name="numberOfLayers"> Number of layers to initialize</param>
	/// <returns></returns>
	bool Initialize(int numberOfLayers);

	/// <summary>
	/// Returns the layers in the system
	/// </summary>
	/// <returns></returns>
	std::vector<std::vector<Entity*>*>  GetLayers() { return listOfLayers; }

	/// <summary>
	/// Adds entity to the chosen layer
	/// </summary>
	/// <param name="ent"> Entity to add</param>
	/// <param name="layer">Chosen layer to add to</param>
	/// <returns></returns>
	bool AddToLayer(Entity* ent, int layer);

	/// <summary>
	/// Remove Entity from the provided layer
	/// </summary>
	/// <param name="ent"></param>
	/// <param name="layer"></param>
	/// <returns></returns>
	bool RemoveFromLayer(Entity* ent, int layer);
	
	/// <summary>
	/// returns the size of the system.
	/// </summary>
	/// <returns></returns>
	int Size() { return size; }
};

#endif // !_ENTITYMANAGERV2_H_