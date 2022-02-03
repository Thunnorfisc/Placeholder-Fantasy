#ifndef _WORLD_H_
#define _WORLD_H_
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "game.h"

struct Boundary
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};

class World : public Image
{
private:
	Boundary bounds;
	TextureManager backgroundImage;
public:

	World();

	bool initialize(Game* gameptr, const char* backgroundTexture, Boundary bounds);

	Boundary retrieveBound() { return bounds; }
};
#endif // !_WORLD_H_
