//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is H file for the Interactable. It is a child of Entity. All objects in  
//                the game is defaulted to Interactable if not a Character. Checks for collision
//				  is in this game object.
//===============================================================================================

#ifndef _INTERACTABLE_H_
#define _INTERACTABLE_H_
#define WIN32_LEAN_AND_MEAN

#include "Entity.h"
#include "Player.h"
#include "EntityManager.h"
#include "EntityManagerV2.h"
#include "SceneManager.h"
#include <vector>

class Interactable : public Entity
{
protected:
	SceneManager* dxManager;
	TextureManager intTexture;
	string destination;
	bool colliding;
public:
	Interactable();

	bool initialize(Game* gameptr, const char* texture);

	virtual bool collideBox(Entity* ent, VECTOR2& collisionVector);

	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector) { return false; }//Not being used

	void triggerBox(Entity* ent, VECTOR2& collisionVector);

	/// <summary>
	/// This allows the layer to be changed in EntityManager (Deprecated)
	/// </summary>
	/// <param name="player"></param>
	/// <param name="layerSystem"></param>
	void triggerLayer(Player* player, EntityManager* layerSystem);

	/// <summary>
	/// This is the latest triggerLayer to work with EntityManagerV2
	/// </summary>
	/// <param name="player"></param>
	/// <param name="layerSystem"></param>
	void triggerLayerV2(Entity* player, EntityManagerV2* layerSystem);

	/// <summary>
	/// Use ONLY for NULL Textures
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	void setSize(float w, float h) { spriteData.width = w; spriteData.height = h; }

	/// <summary>
	/// This is to set scene manager for changing to different scenes
	/// </summary>
	/// <param name="DxManager"></param>
	void setSceneManager(SceneManager* DxManager) { dxManager = DxManager; }
	
	/// <summary>
	/// Used to set what the next scene will be for teleport
	/// </summary>
	/// <param name="dest"></param>
	void setDestination(string dest) { destination = dest; }
	string getDestination() { return destination; }

	virtual void update(float frameTime);
};
#endif // !_Interactable_H_
