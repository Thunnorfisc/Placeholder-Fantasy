#include "Interactable.h"

Interactable::Interactable()
{
    intTexture = TextureManager();
}

bool Interactable::initialize(Game* gameptr, const char* texture)
{
    if (texture != NULL)
        intTexture.initialize(gameptr->getGraphics(), texture);

    Entity::initialize(gameptr, 0, 0, 0, &intTexture);

    return true;
}

bool Interactable::collideBox(Entity& ent, VECTOR2& collisionVector)
{
    if (!active || !ent.getActive()) return false;

    // Check for collision using Axis Aligned Bounding Box.
    if ((getCenterX() + edge.right * getScaleX() < ent.getCenterX() + ent.getEdge().left * ent.getScaleX()) ||
        (getCenterX() + edge.left * getScaleX() > ent.getCenterX() + ent.getEdge().right * ent.getScaleX()) ||
        (getCenterY() + edge.bottom * getScaleY() < ent.getCenterY() + ent.getEdge().top * ent.getScaleY()) ||
        (getCenterY() + edge.top * getScaleY() > ent.getCenterY() + ent.getEdge().bottom * ent.getScaleY()))
    {
        return false;
    }

    // set collision vector
    collisionVector = *ent.getCenter() - *getCenter();
    return true;
}

void Interactable::update(float frameTime)
{

	Entity::update(frameTime);
}