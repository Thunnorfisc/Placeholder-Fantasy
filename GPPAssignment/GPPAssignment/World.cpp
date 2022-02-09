//===============================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Author		: Jason Teo Jun Hao
//	Student ID	: S10194978B
//	Description	: This is CPP file for Creating a World. This is a child of Image and acts as a way
//				  to display the background and also set a boundary that the player can go around
//				  If the world is larger than the window, it will allow the player character to
//			      move freely.
//===============================================================================================

#include "World.h"

World::World()
{
	worldBounds.maxX = 1;
	worldBounds.minX = -1;
	worldBounds.maxY = 1;
	worldBounds.minY = -1;
}

bool World::initialize(Game* gameptr, const char* backgroundTexture)
{
	if (backgroundTexture != NULL)
	{
		backgroundImage.initialize(gameptr->getGraphics(), backgroundTexture);
		return(Image::initialize(gameptr->getGraphics(), 0, 0, 0, &backgroundImage));
	}
	return false;
}

void World::BoundsSetup(float maxX, float minX, float maxY, float minY)
{
	worldBounds.maxX = maxX;
	worldBounds.minX = minX;
	worldBounds.maxY = maxY;
	worldBounds.minY = minY;
}

void World::CheckBoundsCollision(Player* player)
{
	if (player->getX() + player->getWidth() < worldBounds.maxX && player->getX() > worldBounds.minX && player->getY() > worldBounds.minY && player->getY() + player->getHeight() < worldBounds.maxY)
	{
		player->setWorldCollisionVector(VECTOR2(0, 0));

		return;
	}
	if (player->getX() + player->getWidth() > worldBounds.maxX)
	{
		player->setWorldCollisionVector(VECTOR2(player->getX() - worldBounds.maxX,0 + player->getWorldCollisionVector().y));
	}
	else if (player->getX() < worldBounds.minX)
	{
		player->setWorldCollisionVector(VECTOR2(worldBounds.minX - player->getX(), 0 + player->getWorldCollisionVector().y));

	}
	if (player->getY() + player->getHeight() > worldBounds.maxY)
	{
		player->setWorldCollisionVector(VECTOR2(0 + player->getWorldCollisionVector().x, player->getY() - worldBounds.maxY));

	}
	else if (player->getY() < worldBounds.minY)
	{
		player->setWorldCollisionVector(VECTOR2(0 + player->getWorldCollisionVector().x,worldBounds.minY - player->getY()));

	}
}

void World::CheckCameraBound(Player* player, float WorldX, float WorldY)
{
	if (getWidth() <= GAME_WIDTH)
	{
		player->isCenterX(false);
		if (getHeight() <= GAME_HEIGHT)
		{
			player->isCenterY(false);
			return;
		}
		return;
	}
}