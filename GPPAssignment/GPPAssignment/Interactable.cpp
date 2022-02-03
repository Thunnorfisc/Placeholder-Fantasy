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

    setTag("Interactable");

    return true;
}

bool Interactable::collideBox(Entity& ent, VECTOR2& collisionVector)
{
    if (!active || !ent.getActive()) return false;

    float test1 = ent.getCenterX();
    float test2 = ent.getWidth();

    // Check for collision using Axis Aligned Bounding Box.
    if (ent.getX() + ent.getWidth() < getX() ||
        ent.getX() > getX() + getWidth() ||
        ent.getY() + ent.getHeight() < getY() + TRIGGEROFFSET ||
        ent.getY() > getY() + getHeight() - TRIGGEROFFSET)
    {
        return false;
    }

    // set collision vector
    collisionVector = *ent.getCenter() - *getCenter();
    return true;
}

void Interactable::triggerLayer(Player* player, EntityManager* entManager)
{
    if (!active || !player->getActive()) return;

    // Check for trigger on top
    if (player->getX() + player->getWidth() > getX() &&
        player->getX() < getX() + getWidth() &&
        player->getY() + player->getHeight() > getY() &&
        player->getY() + player->getHeight() < getY() + TRIGGEROFFSET)
    {
        if (player->getLayer() > getLayer())
        {
            if (getLayer() != 0)
            {
                //Bring down a layer
                entManager->insert(player, getLayer() - 1);
                entManager->remove(player->getLayer());
            }
            else 
            {
                entManager->insert(this, getLayer() + 1);
                entManager->remove(player->getLayer());
                entManager->insert(player, getLayer() - 1);
                entManager->remove(getLayer());
            }
            
        }

        return;
    }

    // Check for trigger on Bottom
    if (player->getX() + player->getWidth() > getX() &&
        player->getX() < getX() + getWidth() &&
        player->getY() > getY() + getHeight() - TRIGGEROFFSET &&
        player->getY() < getY() + getHeight())
    {
        if (player->getLayer() < getLayer())
        {
            //Bring down a layer
            int oldIndex = player->getLayer();
            entManager->insert(player, getLayer() + 1);
            entManager->remove(oldIndex);
        }

        return;
    }
}

void Interactable::update(float frameTime)
{

	Entity::update(frameTime);
}