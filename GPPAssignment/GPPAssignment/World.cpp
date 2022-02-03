#include "World.h"

World::World()
{
	bounds.maxX = 1;
	bounds.minX = -1;
	bounds.maxY = 1;
	bounds.minY = -1;
}

bool World::initialize(Game* gameptr, const char* backgroundTexture, Boundary Bounds)
{
	if (backgroundTexture != NULL)
	{
		bounds = Bounds;
		backgroundImage.initialize(gameptr->getGraphics(), backgroundTexture);
		return(Image::initialize(gameptr->getGraphics(), 0, 0, 0, &backgroundImage));
	}
	return false;
}