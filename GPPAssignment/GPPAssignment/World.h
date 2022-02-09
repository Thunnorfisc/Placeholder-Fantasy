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

	void BoundsSetup(float minX, float minY, float maxX, float maxY);

	bool initialize(Game* gameptr, const char* backgroundTexture);

	void CheckBoundsCollision(Player* player);

	Boundary* retrieveBound() { return &worldBounds; }

	void CheckCameraBound(Player* player, float WorldX, float WorldY);

};
#endif // !_WORLD_H_
