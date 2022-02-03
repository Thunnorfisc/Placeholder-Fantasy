// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 entity.h v1.0

#ifndef _ENTITY_H               // Prevent multiple definitions if this 
#define _ENTITY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"

namespace entityNS
{
    enum COLLISION_TYPE { NONE, CIRCLE, BOX, ROTATED_BOX };
}

class Entity : public Image
{
    // Entity properties
protected:
    entityNS::COLLISION_TYPE collisionType;
    VECTOR2 center;         // center of entity
    float   sumRadiiSquared;
    // edge specifies the collision box relative to the center of the entity.
    // left and top are typically negative numbers
    RECT    edge;           // for BOX and ROTATED_BOX collision detection
    VECTOR2 edge01, edge03;  // edges used for projection
    float   edge01Min, edge01Max, edge03Min, edge03Max; // min and max projections
    VECTOR2 velocity;       // velocity
    VECTOR2 deltaV;         // added to velocity during next call to update()
    float   mass;           // Mass of entity
    float   rr;             // Radius squared variable
    Input* input;         // pointer to the input system
    HRESULT hr;             // standard return type
    bool    active;         // only active entities may collide
    std::string tag;
    int layer;

    // Axis aligned box collision detection
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideBox(Entity& ent, VECTOR2& collisionVector) = NULL;


public:
    // Constructor
    Entity();
    // Destructor
    virtual ~Entity() {}

    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////

    // Return center of scaled Entity as screen x,y.
    virtual const VECTOR2* getCenter()
    {
        center = VECTOR2(getCenterX(), getCenterY());
        return &center;
    }
    //Set Layer
    virtual void setLayer(int l) { layer = l; }

    //Get Layer
    virtual int getLayer() { return layer; }

    //Set Game Tag
    virtual void setTag(std::string T) { tag = T; }

    virtual std::string getTag() { return tag; }
    // Return RECT structure used for BOX and ROTATED_BOX collision detection.
    virtual const RECT& getEdge() const { return edge; }

    // Return velocity vector.
    virtual const VECTOR2 getVelocity() const { return velocity; }

    // Return active.
    virtual bool  getActive()         const { return active; }

    // Return mass.
    virtual float getMass()           const { return mass; }

    // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }

    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////

    // Set velocity.
    virtual void  setVelocity(VECTOR2 v) { velocity = v; }

    virtual void  setVelocityX(float v) { velocity.x = v; }
    virtual void  setVelocityY(float v) { velocity.y = v; }

    // Set delta velocity. Added to velocity in update().
    virtual void  setDeltaV(VECTOR2 dv) { deltaV = dv; }

    // Set active.
    virtual void  setActive(bool a) { active = a; }

    // Set mass.
    virtual void  setMass(float m) { mass = m; }

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // Update Entity.
    // typically called once per frame
    // frameTime is used to regulate the speed of movement and animation
    virtual void update(float frameTime);

    // Initialize Entity
    // Pre: *gamePtr = pointer to Game object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
    //      *textureM = pointer to TextureManager object
    virtual bool initialize(Game* gamePtr, int width, int height, int ncols,
        TextureManager* textureM);
    // Activate Entity.
    virtual void activate();

    // Empty ai function to allow Entity objects to be instantiated.
    virtual void ai(float frameTime, Entity& ent);

    // Is this entity outside the specified rectangle?
    //virtual bool outsideRect(RECT rect);

    // Does this entity collide with ent?
    virtual bool collidesWith(Entity& ent, VECTOR2& collisionVector);

    // Damage this Entity with weapon.
    //virtual void damage(int weapon);

    // Entity bounces after collision with other Entity
    //void bounce(VECTOR2& collisionVector, Entity& ent);

    // Adds the gravitational force to the velocity vector of this entity
    //void gravityForce(Entity* other, float frameTime);
};

#endif
