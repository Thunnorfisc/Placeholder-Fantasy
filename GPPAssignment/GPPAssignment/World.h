//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is H file for Creating a World. This is a child of Image and acts as a way
//				  to display the background and also set a boundary that the player can go around
//				  If the world is larger than the window, it will allow the player character to
//			      move freely.
//===============================================================================================

#ifndef _WORLD_H_
#define _WORLD_H_
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "game.h"
#include "Player.h"

struct Boundary
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};

//Reminder to multiply bounds by scale
class World : public Image
{
private:
	Boundary worldBounds;
	TextureManager backgroundImage;
public:

	World();

	/// <summary>
	/// Setup bounds that will be used to check if the player can continue moving
	/// </summary>
	/// <param name="minX"></param>
	/// <param name="minY"></param>
	/// <param name="maxX"></param>
	/// <param name="maxY"></param>
	void BoundsSetup(float minX, float minY, float maxX, float maxY);

	/// <summary>
	/// Initialize the world
	/// </summary>
	/// <param name="gameptr"></param>
	/// <param name="backgroundTexture">Image to be used</param>
	/// <returns></returns>
	bool initialize(Game* gameptr, const char* backgroundTexture);

	/// <summary>
	/// Check for the collision of the bounds with the player character
	/// </summary>
	void CheckBoundsCollision(Player* player);

	/// <summary>
	/// Retrieve bounds of the world
	/// </summary>
	Boundary* retrieveBound() { return &worldBounds; }

	void CheckCameraBound(Player* player, float WorldX, float WorldY);

};
#endif // !_WORLD_H_
