#include "Interactable.h"

Interactable::Interactable()
{
    intTexture = TextureManager();
    dxManager = NULL;
    colliding = false;
}

bool Interactable::initialize(Game* gameptr, const char* texture = NULL)
{
    if (texture != NULL)
        intTexture.initialize(gameptr->getGraphics(), texture);

    Entity::initialize(gameptr, 0, 0, 0, &intTexture);

    setClass("Interactable");

    return true;
}

/// <summary>
/// This is to check for the collision of the entity and the current game object.
/// </summary>
/// <param name="ent"></param>
/// <param name="collisionVector"></param>
/// <returns></returns>
bool Interactable::collideBox(Entity* ent, VECTOR2& collisionVector)
{
    
    //if either entity is not active return false
    if (!ent->getActive() || !getActive())
    {
        ent->setCollisionVector(VECTOR2(0, 0));
        colliding = false;
        return false;
    }

    //Check for collision using the Axis Aligned Bounding Box
    if ((getCenterX() + getEdge().right * getWidth() / 2 <= ent->getCenterX() + ent->getEdge().left * ent->getWidth() / 2) ||
        (getCenterX() + getEdge().left * getWidth() / 2 >= ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2) ||
        (getCenterY() + getEdge().bottom * getHeight() / 2 - TRIGGEROFFSET <= ent->getCenterY() + ent->getEdge().top * ent->getHeight() / 2) ||
        (getCenterY() + getEdge().top * getHeight() / 2 + TRIGGEROFFSET >= ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2))
    {
        ent->setCollisionVector(VECTOR2(0, 0));
        colliding = false;
        return false;
    }

    //Test Statements
    //float test5 = ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2;
    //float test6 = getCenterX() + getEdge().left * getWidth() / 2;
    //float test11 = ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2;
    //float test12 = ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2;
    //bool test1= (getCenterX() + getEdge().right * getWidth() / 2 - 1  > ent->getCenterX() + ent->getEdge().left * ent->getWidth() / 2);
    //bool test2= (getCenterX() + getEdge().left * getWidth() / 2  < ent->getCenterX() + ent->getEdge().left * ent->getWidth() / 2);
    //bool test3= (getCenterX() + getEdge().right * getWidth() / 2 > ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2);
    //bool test4= (getCenterX() + getEdge().left * getWidth() / 2 + 1< ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2);

    //bool test7= (getCenterY() + getEdge().bottom * getHeight() / 2 > ent->getCenterY() + ent->getEdge().top * ent->getHeight() / 2);
    //bool test8= (getCenterY() + getEdge().top * getHeight() / 2 < ent->getCenterY() + ent->getEdge().top * ent->getHeight() / 2);
    //bool test9= (getCenterY() + getEdge().bottom * getHeight() / 2 > ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2);
    //bool test10= (getCenterY() + getEdge().top * getHeight() / 2 + 1 < ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2);

    colliding = true;

    //V2
    //Check for collision for Horizontal with Vertical range
    if (((getCenterY() + getEdge().bottom * getHeight() / 2 - TRIGGEROFFSET > ent->getCenterY() + ent->getEdge().top * ent->getHeight() / 2) &&
        (getCenterY() + getEdge().top * getHeight() / 2 + TRIGGEROFFSET< ent->getCenterY() + ent->getEdge().top * ent->getHeight() / 2))
        ||
        ((getCenterY() + getEdge().bottom * getHeight() / 2 - TRIGGEROFFSET > ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2) &&
         (getCenterY() + getEdge().top * getHeight() / 2 + TRIGGEROFFSET< ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2))
        )
    {

        ent->setCollisionVector(VECTOR2(ent->getCenterX() - getCenterX(), 0));
    }
    //Check for collision for Vertical with Horizontal range
    if (((getCenterX() + getEdge().right * getWidth() / 2 - 1 > ent->getCenterX() + ent->getEdge().left * ent->getWidth() / 2) &&
        (getCenterX() + getEdge().left * getWidth() / 2 < ent->getCenterX() + ent->getEdge().left * ent->getWidth() / 2))
        ||
        ((getCenterX() + getEdge().right * getWidth() / 2 > ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2) &&
         (getCenterX() + getEdge().left * getWidth() / 2 + 1< ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2))
        )
    {

        ent->setCollisionVector(VECTOR2(0, ent->getCenterY() - getCenterY()));
    }
    //set collision Vector
    //ent->setCollisionVector(*ent->getCenter() - *getCenter());

    return true;
}

/// <summary>
/// This is to check if the entity is in trigger range with the current game object. Use either this or collideBox
/// </summary>
/// <param name="ent"></param>
/// <param name="collisionVector"></param>
/// <returns></returns>
void Interactable::triggerBox(Entity* ent, VECTOR2& collisionVector)
{
    //Check for collision using the Axis Aligned Bounding Box
    if ((getCenterX() + getEdge().right * getWidth() / 2 <= ent->getCenterX() + ent->getEdge().left * ent->getWidth() / 2) ||
        (getCenterX() + getEdge().left * getWidth() / 2 >= ent->getCenterX() + ent->getEdge().right * ent->getWidth() / 2) ||
        (getCenterY() + getEdge().bottom * getHeight() / 2 - TRIGGEROFFSET <= ent->getCenterY() + ent->getEdge().top * ent->getHeight() / 2) ||
        (getCenterY() + getEdge().top * getHeight() / 2 + TRIGGEROFFSET >= ent->getCenterY() + ent->getEdge().bottom * ent->getHeight() / 2))
    {
        colliding = false;
        return;
    }

     colliding = true;
}

/// <summary>
/// This is used to swap the layers for gameObjects in the game using the entityManager V1(deprecated) 
/// </summary>
/// <param name="player"></param>
/// <param name="entManager"></param>
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

/// <summary>
/// This is used to swap the position of a gameObject that has triggered the trigger by moving Entity from one layer to another
/// </summary>
/// <param name="ent"></param>
/// <param name="entManagerV2"></param>
void Interactable::triggerLayerV2(Entity* ent, EntityManagerV2* entManagerV2)
{
    if (!active || !ent->getActive()) return;

    // Check for trigger on top
    if (ent->getX() + ent->getWidth() > getX() &&
        ent->getX() < getX() + getWidth() &&
        ent->getY() + ent->getHeight() > getY() &&
        ent->getY() + ent->getHeight() < getY() + TRIGGEROFFSET)
    {
        //Bring Back
        if (ent->getLayer() >= getLayer() && getLayer() - 1 >= 0)
        {
            entManagerV2->RemoveFromLayer(ent, ent->getLayer());
            entManagerV2->AddToLayer(ent, getLayer() - 1);
        }

        return;
    }

    // Check for trigger on Bottom
    if (ent->getX() + ent->getWidth() > getX() &&
        ent->getX() < getX() + getWidth() &&
        ent->getY() > getY() + getHeight() - TRIGGEROFFSET &&
        ent->getY() < getY() + getHeight())
    {

        //Bring forward
        if (ent->getLayer() < getLayer() )
        {
            //Bring down a layer
            entManagerV2->RemoveFromLayer(ent, ent->getLayer());
            entManagerV2->AddToLayer(ent, getLayer());
        }

        return;
    }
}

void Interactable::update(float frameTime)
{
    if (colliding && input->isKeyDown(VK_SPACE))
    {
        if (tag == "tpObj" && dxManager != NULL)
        {
            dxManager->switchScene("Title");
        }

    }
	Entity::update(frameTime);
}