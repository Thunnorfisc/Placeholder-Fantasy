#include "EntityManagerV2.h"

EntityManagerV2::EntityManagerV2()
{
	size = 0;
	listOfLayers = std::vector<std::vector<Entity*>*>();
}

EntityManagerV2::~EntityManagerV2()
{
	size = 0;
	listOfLayers.clear();
}

bool EntityManagerV2::Initialize(int numberOfLayers)
{
	if (numberOfLayers <= 0 || numberOfLayers > MAXLAYERS)
	{
		return false;
	}

	for (int i = 0; i < numberOfLayers; i++)
	{
		std::vector<Entity*>* newLayer = new std::vector<Entity*>();
		listOfLayers.push_back(newLayer);
	}
	size = numberOfLayers;
	return true;
}

bool EntityManagerV2::AddToLayer(Entity* ent, int layer)
{
	if (layer < 0 || layer > MAXLAYERS) return false;
	listOfLayers.at(layer)->push_back(ent);
	ent->setLayer(layer);
	return true;
}

bool EntityManagerV2::RemoveFromLayer(Entity* ent, int layer)
{
	if (layer <= 0 || layer > MAXLAYERS) return false;
	for (int i = 0; i< listOfLayers.at(layer)->size(); i++)
	{
		if (listOfLayers.at(layer)->at(i) == ent) 
			listOfLayers.at(layer)->erase(listOfLayers.at(layer)->begin() + i);
	}
	return true;
}
