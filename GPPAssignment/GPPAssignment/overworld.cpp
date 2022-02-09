#include "Overworld.h"

Overworld::Overworld(SceneManager* manager) 
{
	dxManager = manager;
	world = World();
	player = Player();
	npc1 = NPC();
	cliff_1 = Interactable();
	cliff_2 = Interactable();
	cliff_3 = Interactable();
	wagon = Interactable();
	waterfall = Interactable();

	WorldX = 0;
	WorldY = 0;

	screenHeight = GAME_HEIGHT;
	screenWidth = GAME_WIDTH;
}

Overworld::~Overworld()
{
	dxManager = NULL;
}

void Overworld::initialize()
{
	//Initialize WorldX and Y
	setWorldPos(0, 0, &world);

	if (!TPOBJECT.initialize(dxManager, NULL)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize TP"));
	TPOBJECT.setSize(OVERWORLD_TP_WIDTH, OVERWORLD_TP_HEIGHT);
	TPOBJECT.setPos(getWorldX() + OVERWORLD_TP_X * OVERWORLD_SCALE, getWorldY() + OVERWORLD_TP_Y * OVERWORLD_SCALE);
	TPOBJECT.setTag("tpObj");
	TPOBJECT.setSceneManager(dxManager);
	TPOBJECT.setDestination("StartingRoom");

	//Initialize Layering System
	if (!entManagerV2.Initialize(MAXLAYERS)) throw(GameError(gameErrorNS::FATAL_ERROR, "Invalid Max Layers"));

	//initialize World
	if(!world.initialize(dxManager,OVERWORLD_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR,"Failed to initialize world"));

	//Set Parameters
	world.setScale(OVERWORLD_SCALE);
	world.setPos(getWorldX(), getWorldY());
	world.BoundsSetup(getWorldX() + world.getWidth(), getWorldX(), getWorldY() + world.getHeight() * OVERWORLDBOUND_MAXY, getWorldY());

	//initialize cliffs
	if (!cliff_1.initialize(dxManager, CLIFF_1_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Cliff_1"));
	if (!cliff_2.initialize(dxManager, CLIFF_2_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Cliff_2"));
	if (!cliff_3.initialize(dxManager, CLIFF_3_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Cliff_3"));

	//Set Cliff Parameters
	cliff_1.setPos(getWorldX() + CLIFF_1_POSX * OVERWORLD_SCALE, getWorldY() + CLIFF_1_POSY * OVERWORLD_SCALE);
	cliff_2.setPos(getWorldX() + CLIFF_2_POSX * OVERWORLD_SCALE, getWorldY() + CLIFF_2_POSY * OVERWORLD_SCALE);
	cliff_3.setPos(getWorldX() + CLIFF_3_POSX * OVERWORLD_SCALE, getWorldY() + CLIFF_3_POSY * OVERWORLD_SCALE);

	//initialize wagon
	if (!wagon.initialize(dxManager, WAGON_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Wagon"));
	
	//Set Wagon Parameters
	wagon.setPos(getWorldX() + WAGON_POSX * OVERWORLD_SCALE, getWorldY() + WAGON_POSY * OVERWORLD_SCALE);

	//initialize waterfall
	if (!waterfall.initialize(dxManager, WATERFALL_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Waterfall"));
	
	//Set Waterfall Parameters
	waterfall.setPos(getWorldX() + WATERFALL_POSX * OVERWORLD_SCALE, getWorldY() + WATERFALL_POSY * OVERWORLD_SCALE);

	//initialize Player
	if (!player.initialize(dxManager, MAINCHARA_ANIMATION, MAINCHARA_IMAGE, CHARA_SIZE, CHARA_SIZE, 0)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize player"));
	player.setScale(CHARA_SCALE);
	player.setPos(screenWidth/2 - player.getWidth(), screenHeight / 2 - player.getHeight() / 2);
	player.setFrame(0, 4);

	//Add Game Objects to Layers
	entManagerV2.AddToLayer(&cliff_1,MAXLAYERS - 1);
	entManagerV2.AddToLayer(&cliff_2,MAXLAYERS - 1);
	entManagerV2.AddToLayer(&cliff_3,MAXLAYERS - 1);
	entManagerV2.AddToLayer(&wagon,MAXLAYERS - 2);
	entManagerV2.AddToLayer(&waterfall,MAXLAYERS - 2);
	entManagerV2.AddToLayer(&player,MAXLAYERS - 2);
	entManagerV2.AddToLayer(&TPOBJECT,MAXLAYERS - MAXLAYERS);

	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			if(layer->at(i)->getClass() == "Interactable")
			layer->at(i)->setScale(OVERWORLD_SCALE);
		}
	}

}

void Overworld::update(float frameTime) 
{
	if (player.ifCenterX())
	{
		if (collisionVector.x != 0 || player.getWorldCollisionVector().x != 0)
		{
			collisionVector.x += isinf(worldVelocityX / collisionVector.x) * worldVelocityX;

			float testInf = player.getWorldCollisionVector().x;
			if (isinf(worldVelocityX / player.getWorldCollisionVector().x)) testInf = isinf(worldVelocityX / player.getWorldCollisionVector().x) * worldVelocityX;

			if (worldVelocityX / collisionVector.x < 0 || worldVelocityX / testInf < 0)
			{
				setWorldPos(getWorldX() + worldVelocityX * frameTime, getWorldY(), &world);
				collisionVector.x = 0;
			}
		}
		else {
			setWorldPos(getWorldX() + worldVelocityX * frameTime, getWorldY(), &world);
		}

	}
	if (player.ifCenterY())
	{
		if (collisionVector.y != 0 || player.getWorldCollisionVector().y != 0)
		{
			collisionVector.y += isinf(worldVelocityY / collisionVector.y) * worldVelocityY;

			float testInf = player.getWorldCollisionVector().y;
			if (isinf(worldVelocityY / player.getWorldCollisionVector().y)) testInf = isinf(worldVelocityY / player.getWorldCollisionVector().y) * worldVelocityY;

			if (worldVelocityY / collisionVector.y < 0 || worldVelocityY / testInf < 0)
			{
				setWorldPos(getWorldX(), getWorldY() + worldVelocityY * frameTime, &world);
				collisionVector.y = 0;
			}
		}
		else {
			setWorldPos(getWorldX(), getWorldY() + worldVelocityY * frameTime, &world);
		}
	}

	if (dxManager->getInput()->isKeyDown(VK_LEFT))
	{
		worldVelocityX = MOVEMENTSPEED;
	}	
	else if (dxManager->getInput()->isKeyDown(VK_RIGHT))
	{
		worldVelocityX = -MOVEMENTSPEED;
	}
	else { worldVelocityX = 0; }

	if (dxManager->getInput()->isKeyDown(VK_UP))
	{
		worldVelocityY = MOVEMENTSPEED;
	}	
	else if (dxManager->getInput()->isKeyDown(VK_DOWN))
	{
		worldVelocityY = -MOVEMENTSPEED;
	}
	else { worldVelocityY = 0; }

	//V2 TriggerLayers
	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			layer->at(i)->update(frameTime);
			if (layer->at(i)->getClass() == "Interactable")
			{
				
				updateWorldObj(layer->at(i));
			}
		}
	}
	world.BoundsSetup(getWorldX() + world.getWidth(), getWorldX(), getWorldY() + world.getHeight() * OVERWORLDBOUND_MAXY, getWorldY());

}

void Overworld::ai() {}

void Overworld::collisions() 
{

	world.CheckBoundsCollision(&player);

	//V2 TriggerLayers
	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			if (layer->at(i)->getClass() == "Interactable")
			{
				Interactable* interactable = dynamic_cast<Interactable*> (layer->at(i));
				interactable->triggerLayerV2(&player, &entManagerV2);
				if (interactable->collideBox(&player, collisionVector))
				{
					return;
				}

				interactable->triggerBox(&player, collisionVector);
			}
		}
	}
}

void Overworld::render() 
{
	dxManager->getGraphics()->spriteBegin();

	//Draw the world map
	world.draw(TRANSCOLOR);

	//Here is where the layer system go
	//V2 System
	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			layer->at(i)->draw(TRANSCOLOR);
		}
	}

	dxManager->getGraphics()->spriteEnd();
}

void Overworld::releaseAll() {}

void Overworld::resetAll() {}

void Overworld::onMessage(const Mail& mail) {}

void Overworld::setWorldPos(float x, float y, World* w)
{
	WorldX = x; 
	WorldY = y;
	world.setX(x);
	world.setY(y);
}

void Overworld::updateWorldObj(Entity* ent)
{
	ent->setX(getWorldX() + ent->getOriX());
	ent->setY(getWorldY() + ent->getOriY());
}