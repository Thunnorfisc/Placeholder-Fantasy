#include "Entity.h"

Entity::Entity() 
{
	collisionType = entityNS::BOX;
	edge.left = -1;
	edge.top = -1;
	edge.right = 1;
	edge.bottom = 1;
	mass = 1.0;
	velocity.x = 0.0;
	velocity.y = 0.0;

	deltaV.x = 0.0;
	deltaV.y = 0.0;
	active = true;
	tag = "";
}

bool Entity::initialize(Game* gameptr, int width, int height, int ncols, TextureManager* textureM)
{
	input = gameptr->getInput();
	return(Image::initialize(gameptr->getGraphics(), width, height, ncols, textureM));
}

void Entity::activate() { active = true; }

void Entity::update(float frameTime) 
{

	Image::update(frameTime);
}

void Entity::ai(float frameTime, Entity &ent)
{}

bool Entity::collidesWith(Entity& ent, VECTOR2& collisionVector)
{
	if (!active || !ent.getActive()) return false;

	if (collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
	{
		return collideBox(ent, collisionVector);
	}

	return false;
}
