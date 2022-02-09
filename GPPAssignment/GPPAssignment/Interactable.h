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

	void triggerLayer(Player* player, EntityManager* layerSystem);

	void triggerLayerV2(Entity* player, EntityManagerV2* layerSystem);

	/// <summary>
	/// Use ONLY for NULL Textures
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	void setSize(float w, float h) { spriteData.width = w; spriteData.height = h; }

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
