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
	worldBounds.maxX = maxX * getScaleX();
	worldBounds.minX = minX * getScaleX();
	worldBounds.maxY = maxY * getScaleY();
	worldBounds.minY = minY * getScaleY();
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